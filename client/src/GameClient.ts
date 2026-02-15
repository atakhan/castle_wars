import * as PIXI from 'pixi.js';
import { Client, Room } from 'colyseus.js';
import { GameState, Player, StaticEntity } from './schemas/GameState';

// Структура для хранения истории состояний (для interpolation)
interface Snapshot {
  x: number;
  y: number;
  vx: number;
  vy: number;
  direction: number;
  timestamp: number;
}

// Структура для хранения отправленных команд (для reconciliation)
interface InputCommand {
  seq: number;
  moveX: number;
  moveY: number;
  timestamp: number;
}

// Структура для локального состояния игрока (для prediction)
interface LocalPlayerState {
  x: number;
  y: number;
  vx: number;
  vy: number;
  direction: number;
}

export class GameClient {
  private client: Client;
  private room: Room<GameState> | null = null;
  private players: Map<string, PIXI.Graphics> = new Map();
  private playerContainer: PIXI.Container;
  private obstacleContainer: PIXI.Container;
  private staticEntityGraphics: Map<string, PIXI.Graphics> = new Map();
  
  // Input handling
  private inputSequence: number = 0;
  private lastInputTime: number = 0;
  private readonly INPUT_RATE = 20; // 20 inputs per second
  private keys: Set<string> = new Set();
  private lastMoveX: number = 0;
  private lastMoveY: number = 0;
  
  // Client-side prediction для своего игрока
  private localPlayerState: LocalPlayerState | null = null;
  private pendingInputs: InputCommand[] = []; // История отправленных команд
  private readonly MOVE_SPEED = 200; // Должно совпадать с сервером
  
  // Interpolation для других игроков
  private playerSnapshots: Map<string, Snapshot[]> = new Map(); // История состояний для каждого игрока
  private readonly INTERPOLATION_DELAY = 100; // Задержка для interpolation (мс)
  private readonly MAX_SNAPSHOTS = 10; // Максимум состояний в истории
  
  // Reconciliation
  private lastServerState: Map<string, { x: number; y: number; vx: number; vy: number }> = new Map();
  
  public onStatusChange?: (status: string) => void;
  public onPlayersChange?: (count: number) => void;
  public onInteractHintChange?: (show: boolean) => void;
  
  private mySessionId: string = '';
  private readonly INTERACT_RADIUS = 60;

  constructor(private app: PIXI.Application, private serverUrl: string) {
    this.obstacleContainer = new PIXI.Container();
    this.playerContainer = new PIXI.Container();
    this.app.stage.addChild(this.obstacleContainer);
    this.app.stage.addChild(this.playerContainer);
    
    this.client = new Client(serverUrl);
    
    // Setup input handling
    this.setupInput();
  }

  async start() {
    try {
      this.updateStatus('Connecting...');
      
      this.room = await this.client.joinOrCreate<GameState>('game');
      this.mySessionId = this.room.sessionId;
      
      this.updateStatus('Connected');
      this.setupRoomHandlers();
      
    } catch (error) {
      console.error('Failed to connect:', error);
      this.updateStatus('Connection failed');
    }
  }

  private setupRoomHandlers() {
    if (!this.room) return;

    // Handle state changes
    this.room.state.players.onAdd((player: Player, sessionId: string) => {
      console.log('Player joined:', sessionId);
      this.createPlayerVisualization(sessionId, player);
      this.updatePlayersCount();
      
      // Инициализация для interpolation
      if (sessionId !== this.mySessionId) {
        this.playerSnapshots.set(sessionId, []);
        this.addSnapshot(sessionId, player);
      } else {
        // Инициализация локального состояния для prediction
        this.localPlayerState = {
          x: player.x,
          y: player.y,
          vx: player.vx,
          vy: player.vy,
          direction: player.direction
        };
        this.lastServerState.set(sessionId, {
          x: player.x,
          y: player.y,
          vx: player.vx,
          vy: player.vy
        });
      }
    });

    this.room.state.players.onRemove((player: Player, sessionId: string) => {
      console.log('Player left:', sessionId);
      this.removePlayerVisualization(sessionId);
      this.playerSnapshots.delete(sessionId);
      this.lastServerState.delete(sessionId);
      this.updatePlayersCount();
    });

    // Static obstacles (walls, boxes)
    const state = this.room.state as GameState & {
      staticEntities?: {
        onAdd: (cb: (entity: StaticEntity, key: string) => void) => void;
        forEach: (cb: (entity: StaticEntity, key: string) => void) => void;
      };
    };
    if (state.staticEntities) {
      state.staticEntities.onAdd((entity: StaticEntity, key: string) => {
        this.createObstacleVisualization(key, entity);
      });
      // Draw obstacles that already exist when we joined (onAdd may not fire for them)
      state.staticEntities.forEach((entity: StaticEntity, key: string) => {
        this.createObstacleVisualization(key, entity);
      });
    }

    // Handle player updates - используем onChange для каждого игрока
    this.room.state.players.onAdd((player: Player, sessionId: string) => {
      // Слушаем изменения свойств игрока
      player.onChange(() => {
        this.handlePlayerStateUpdate(sessionId, player);
      });
      
      // Также слушаем изменения отдельных полей для более точного отслеживания
      player.listen('x', () => this.handlePlayerStateUpdate(sessionId, player));
      player.listen('y', () => this.handlePlayerStateUpdate(sessionId, player));
      player.listen('vx', () => this.handlePlayerStateUpdate(sessionId, player));
      player.listen('vy', () => this.handlePlayerStateUpdate(sessionId, player));
    });

    // Handle connection state changes
    this.room.onLeave(() => {
      console.log('Room connection closed');
      this.updateStatus('Disconnected');
      this.room = null;
      this.localPlayerState = null;
      this.pendingInputs = [];
      this.playerSnapshots.clear();
      this.removeAllObstacles();
    });

    this.room.onError((code: number, message: string) => {
      console.error('Room error:', code, message);
      this.updateStatus('Connection error');
    });

    // Start render loop (60 FPS)
    this.app.ticker.add(this.update.bind(this));
  }

  private handlePlayerStateUpdate(sessionId: string, player: Player) {
    if (sessionId === this.mySessionId) {
      // Это наш игрок - выполняем reconciliation
      this.reconcilePlayerState(player);
    } else {
      // Это другой игрок - добавляем snapshot для interpolation
      this.addSnapshot(sessionId, player);
    }
  }

  private addSnapshot(sessionId: string, player: Player) {
    const snapshots = this.playerSnapshots.get(sessionId) || [];
    snapshots.push({
      x: player.x,
      y: player.y,
      vx: player.vx,
      vy: player.vy,
      direction: player.direction,
      timestamp: Date.now()
    });
    
    // Ограничиваем размер истории
    if (snapshots.length > this.MAX_SNAPSHOTS) {
      snapshots.shift();
    }
    
    this.playerSnapshots.set(sessionId, snapshots);
  }

  private reconcilePlayerState(serverPlayer: Player) {
    // Сохраняем серверное состояние
    const serverState = {
      x: serverPlayer.x,
      y: serverPlayer.y,
      vx: serverPlayer.vx,
      vy: serverPlayer.vy
    };
    
    const lastState = this.lastServerState.get(this.mySessionId);
    
    // Если есть расхождение с локальным состоянием, корректируем
    if (this.localPlayerState && lastState) {
      const dx = Math.abs(this.localPlayerState.x - serverState.x);
      const dy = Math.abs(this.localPlayerState.y - serverState.y);
      const threshold = 5; // Порог для reconciliation (пикселей)
      
      if (dx > threshold || dy > threshold) {
        // Есть расхождение - применяем reconciliation
        console.log(`[Reconciliation] Correcting position: local (${this.localPlayerState.x.toFixed(2)}, ${this.localPlayerState.y.toFixed(2)}) -> server (${serverState.x.toFixed(2)}, ${serverState.y.toFixed(2)})`);
        
        // Устанавливаем серверное состояние как базовое
        this.localPlayerState.x = serverState.x;
        this.localPlayerState.y = serverState.y;
        this.localPlayerState.vx = serverState.vx;
        this.localPlayerState.vy = serverState.vy;
        
        // Переигрываем все pending inputs после серверного состояния
        this.replayPendingInputs();
      } else {
        // Небольшое расхождение - просто обновляем локальное состояние
        this.localPlayerState.x = serverState.x;
        this.localPlayerState.y = serverState.y;
        this.localPlayerState.vx = serverState.vx;
        this.localPlayerState.vy = serverState.vy;
      }
    } else {
      // Первое обновление - просто устанавливаем состояние
      if (!this.localPlayerState) {
        this.localPlayerState = {
          x: serverState.x,
          y: serverState.y,
          vx: serverState.vx,
          vy: serverState.vy,
          direction: serverPlayer.direction
        };
      } else {
        this.localPlayerState.x = serverState.x;
        this.localPlayerState.y = serverState.y;
        this.localPlayerState.vx = serverState.vx;
        this.localPlayerState.vy = serverState.vy;
      }
    }
    
    // Удаляем подтвержденные команды
    this.removeAcknowledgedInputs();
    
    this.lastServerState.set(this.mySessionId, serverState);
  }

  private replayPendingInputs() {
    if (!this.localPlayerState) return;
    
    // Переигрываем все pending inputs
    for (const input of this.pendingInputs) {
      const moveX = input.moveX * this.MOVE_SPEED;
      const moveY = input.moveY * this.MOVE_SPEED;
      
      // Применяем движение (упрощенная версия, без deltaTime, так как это переигрывание)
      // В реальности нужно учитывать время между командами
      this.localPlayerState.vx = moveX;
      this.localPlayerState.vy = moveY;
      
      if (moveX !== 0 || moveY !== 0) {
        this.localPlayerState.direction = Math.atan2(moveY, moveX);
      }
    }
  }

  private removeAcknowledgedInputs() {
    // Удаляем старые команды (старше 200мс, так как сервер тикает на 20 TPS = 50мс)
    const now = Date.now();
    this.pendingInputs = this.pendingInputs.filter(input => 
      now - input.timestamp < 200
    );
  }

  private setupInput() {
    window.addEventListener('keydown', (e) => {
      const key = e.key.toLowerCase();
      if (['w', 'a', 's', 'd', 'arrowup', 'arrowdown', 'arrowleft', 'arrowright'].includes(key)) {
        e.preventDefault();
        this.keys.add(key);
      }
      if (key === 'e') {
        e.preventDefault();
        this.sendInteract();
      }
    });
    
    window.addEventListener('keyup', (e) => {
      const key = e.key.toLowerCase();
      if (['w', 'a', 's', 'd', 'arrowup', 'arrowdown', 'arrowleft', 'arrowright'].includes(key)) {
        e.preventDefault();
        this.keys.delete(key);
        this.handleInput(true);
      }
    });
  }

  private sendInteract() {
    if (!this.room) return;
    try {
      this.room.send('interact');
    } catch {
      // connection closed
    }
  }

  private handleInput(force: boolean = false) {
    if (!this.room) {
      return;
    }

    const now = Date.now();
    const minInterval = 1000 / this.INPUT_RATE;
    
    if (!force && now - this.lastInputTime < minInterval) {
      return;
    }
    
    this.lastInputTime = now;

    // Calculate movement vector
    let moveX = 0;
    let moveY = 0;

    if (this.keys.has('w') || this.keys.has('arrowup')) moveY -= 1;
    if (this.keys.has('s') || this.keys.has('arrowdown')) moveY += 1;
    if (this.keys.has('a') || this.keys.has('arrowleft')) moveX -= 1;
    if (this.keys.has('d') || this.keys.has('arrowright')) moveX += 1;

    // Normalize diagonal movement
    if (moveX !== 0 && moveY !== 0) {
      moveX *= 0.707;
      moveY *= 0.707;
    }

    // Only send if input changed
    if (moveX === this.lastMoveX && moveY === this.lastMoveY && !force) {
      return;
    }

    this.lastMoveX = moveX;
    this.lastMoveY = moveY;

    // Сохраняем команду для reconciliation
    const command: InputCommand = {
      seq: this.inputSequence++,
      moveX,
      moveY,
      timestamp: now
    };
    this.pendingInputs.push(command);

    // Применяем локальное предсказание (client-side prediction)
    this.applyLocalPrediction(moveX, moveY);

    // Send input to server
    try {
      this.room.send('input', {
        seq: command.seq,
        ts: now,
        moveX,
        moveY,
      });
    } catch (error) {
      // Connection might be closed
    }
  }

  private applyLocalPrediction(moveX: number, moveY: number) {
    if (!this.localPlayerState) return;
    
    // Применяем движение локально (предсказание)
    const vx = moveX * this.MOVE_SPEED;
    const vy = moveY * this.MOVE_SPEED;
    
    this.localPlayerState.vx = vx;
    this.localPlayerState.vy = vy;
    
    if (vx !== 0 || vy !== 0) {
      this.localPlayerState.direction = Math.atan2(vy, vx);
    }
  }

  private createPlayerVisualization(sessionId: string, player: Player) {
    const graphics = new PIXI.Graphics();
    const isMe = sessionId === this.mySessionId;
    
    // Разные цвета для своего и чужих игроков
    graphics.beginFill(isMe ? 0x00ff00 : 0x0088ff); // Зеленый для себя, синий для других
    graphics.drawCircle(0, 0, 30);
    graphics.endFill();
    
    graphics.lineStyle(2, isMe ? 0x00aa00 : 0x0066aa);
    graphics.drawCircle(0, 0, 30);
    
    // Добавляем стрелку для направления
    graphics.lineStyle(3, isMe ? 0xffffff : 0xffffff);
    graphics.moveTo(0, 0);
    graphics.lineTo(25, 0);
    graphics.endFill();
    
    const centerX = this.app.screen.width / 2;
    const centerY = this.app.screen.height / 2;
    graphics.x = player.x + centerX;
    graphics.y = player.y + centerY;
    graphics.rotation = player.direction;
    
    graphics.visible = true;
    graphics.alpha = 1.0;
    
    this.players.set(sessionId, graphics);
    this.playerContainer.addChild(graphics);
  }

  private updatePlayerVisualization(sessionId: string, player: Player) {
    const graphics = this.players.get(sessionId);
    if (!graphics) return;
    
    const centerX = this.app.screen.width / 2;
    const centerY = this.app.screen.height / 2;
    
    if (sessionId === this.mySessionId) {
      // Для своего игрока используем локальное предсказание
      if (this.localPlayerState) {
        graphics.x = this.localPlayerState.x + centerX;
        graphics.y = this.localPlayerState.y + centerY;
        graphics.rotation = this.localPlayerState.direction;
      }
    } else {
      // Для других игроков используем interpolation
      const interpolated = this.interpolatePlayerState(sessionId);
      if (interpolated) {
      graphics.x = interpolated.x + centerX;
      graphics.y = interpolated.y + centerY;
      graphics.rotation = interpolated.direction;
      } else {
        // Fallback на прямое обновление
        graphics.x = player.x + centerX;
        graphics.y = player.y + centerY;
        graphics.rotation = player.direction;
      }
    }
  }

  private interpolatePlayerState(sessionId: string): { x: number; y: number; direction: number } | null {
    const snapshots = this.playerSnapshots.get(sessionId);
    if (!snapshots || snapshots.length < 2) {
      return null;
    }
    
    const now = Date.now();
    const targetTime = now - this.INTERPOLATION_DELAY;
    
    // Находим два ближайших snapshot'а
    let older: Snapshot | null = null;
    let newer: Snapshot | null = null;
    
    for (let i = snapshots.length - 1; i >= 0; i--) {
      const snap = snapshots[i];
      if (snap.timestamp <= targetTime) {
        older = snap;
        if (i < snapshots.length - 1) {
          newer = snapshots[i + 1];
        }
        break;
      }
    }
    
    if (!older || !newer) {
      // Используем самый новый snapshot
      const latest = snapshots[snapshots.length - 1];
      return {
        x: latest.x,
        y: latest.y,
        direction: latest.direction
      };
    }
    
    // Интерполируем между двумя snapshot'ами
    const timeDiff = newer.timestamp - older.timestamp;
    if (timeDiff === 0) {
      return {
        x: newer.x,
        y: newer.y,
        direction: newer.direction
      };
    }
    
    const t = (targetTime - older.timestamp) / timeDiff;
    const clampedT = Math.max(0, Math.min(1, t)); // Ограничиваем от 0 до 1
    
    return {
      x: older.x + (newer.x - older.x) * clampedT,
      y: older.y + (newer.y - older.y) * clampedT,
      direction: older.direction + (newer.direction - older.direction) * clampedT
    };
  }

  private removePlayerVisualization(sessionId: string) {
    const graphics = this.players.get(sessionId);
    if (graphics) {
      this.playerContainer.removeChild(graphics);
      graphics.destroy();
      this.players.delete(sessionId);
    }
  }

  private createObstacleVisualization(id: string, entity: StaticEntity) {
    // Avoid duplicate if already created (e.g. both onAdd and forEach)
    const existing = this.staticEntityGraphics.get(id);
    if (existing) {
      this.obstacleContainer.removeChild(existing);
      existing.destroy();
      this.staticEntityGraphics.delete(id);
    }
    const centerX = this.app.screen.width / 2;
    const centerY = this.app.screen.height / 2;
    const graphics = new PIXI.Graphics();
    const isInteractable = !!entity.interactableKind;
    const isOpen = entity.interactableState === "open";
    const fill = isInteractable ? (isOpen ? 0x5c4a32 : 0x8b6914) : 0x4a3728;
    const line = isInteractable ? (isOpen ? 0x3d3510 : 0x6b5a20) : 0x2a1810;
    graphics.beginFill(fill);
    graphics.lineStyle(2, line);
    if (entity.shape === "circle") {
      graphics.drawCircle(0, 0, entity.radius);
    } else {
      graphics.drawRect(-entity.halfWidth, -entity.halfHeight, entity.halfWidth * 2, entity.halfHeight * 2);
    }
    graphics.endFill();
    graphics.x = entity.x + centerX;
    graphics.y = entity.y + centerY;
    this.staticEntityGraphics.set(id, graphics);
    this.obstacleContainer.addChild(graphics);
    if (entity.interactableKind && typeof (entity as { listen?: (field: string, cb: () => void) => void }).listen === "function") {
      (entity as { listen: (field: string, cb: () => void) => void }).listen("interactableState", () => {
        this.updateObstacleVisualization(id, entity);
      });
    }
  }

  private updateObstacleVisualization(id: string, entity: StaticEntity) {
    const graphics = this.staticEntityGraphics.get(id);
    if (!graphics) return;
    graphics.clear();
    const isInteractable = !!entity.interactableKind;
    const isOpen = entity.interactableState === "open";
    const fill = isInteractable ? (isOpen ? 0x5c4a32 : 0x8b6914) : 0x4a3728;
    const line = isInteractable ? (isOpen ? 0x3d3510 : 0x6b5a20) : 0x2a1810;
    graphics.beginFill(fill);
    graphics.lineStyle(2, line);
    if (entity.shape === "circle") {
      graphics.drawCircle(0, 0, entity.radius);
    } else {
      graphics.drawRect(-entity.halfWidth, -entity.halfHeight, entity.halfWidth * 2, entity.halfHeight * 2);
    }
    graphics.endFill();
  }

  private removeAllObstacles() {
    this.staticEntityGraphics.forEach((g) => {
      this.obstacleContainer.removeChild(g);
      g.destroy();
    });
    this.staticEntityGraphics.clear();
  }

  private update(deltaTime: number) {
    // Обновляем локальное предсказание (движение своего игрока)
    if (this.localPlayerState && this.room) {
      // PixiJS ticker дает deltaTime как множитель (1.0 = 60 FPS)
      // Конвертируем в секунды: deltaTime / 60
      const dt = deltaTime / 60;
      
      // Применяем velocity к позиции
      this.localPlayerState.x += this.localPlayerState.vx * dt;
      this.localPlayerState.y += this.localPlayerState.vy * dt;
      
      // Обновляем визуализацию
      const myPlayer = this.room.state.players.get(this.mySessionId);
      if (myPlayer) {
        this.updatePlayerVisualization(this.mySessionId, myPlayer);
      }
    }
    
    // Обновляем визуализацию других игроков (interpolation)
    if (this.room) {
      this.room.state.players.forEach((player, sessionId) => {
        if (sessionId !== this.mySessionId) {
          this.updatePlayerVisualization(sessionId, player);
        }
      });
    }
    
    // Handle continuous input
    if (this.keys.size > 0 && this.room) {
      this.handleInput();
    }

    // Interact hint: show "E - interact" when near an interactable
    if (this.room && this.localPlayerState) {
      const state = this.room.state as GameState & { staticEntities?: { forEach: (cb: (entity: StaticEntity, key: string) => void) => void } };
      let showHint = false;
      if (state.staticEntities) {
        state.staticEntities.forEach((entity: StaticEntity, _key: string) => {
          if (entity.interactableKind && entity.interactableState === "closed") {
            const dx = entity.x - this.localPlayerState!.x;
            const dy = entity.y - this.localPlayerState!.y;
            if (dx * dx + dy * dy <= this.INTERACT_RADIUS * this.INTERACT_RADIUS) showHint = true;
          }
        });
      }
      this.onInteractHintChange?.(showHint);
    }
  }

  private updateStatus(status: string) {
    if (this.onStatusChange) {
      this.onStatusChange(status);
    }
  }

  private updatePlayersCount() {
    if (this.onPlayersChange) {
      this.onPlayersChange(this.players.size);
    }
  }
}
