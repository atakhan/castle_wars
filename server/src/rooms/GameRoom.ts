import { Room, Client } from "colyseus";
import { GameState, Player } from "../schemas/GameState";
import { GameWorld } from "../ecs/GameWorld";

export class GameRoom extends Room<GameState> {
  private world: GameWorld;
  private tickInterval: NodeJS.Timeout | null = null;
  private readonly TICK_RATE = 20; // 20 TPS

  onCreate(options: any) {
    console.log("GameRoom created");
    
    this.setState(new GameState());
    this.world = new GameWorld();
    
    // Register message handler for 'input' type
    // This MUST be done in onCreate for Colyseus 0.15.0
    const handler = (client: Client, message: any) => {
      console.log(`[onMessage 'input'] Input from ${client.sessionId}:`, message);
      try {
        this.world.handlePlayerInput(client.sessionId, message);
      } catch (error) {
        console.error(`Error handling input from ${client.sessionId}:`, error);
      }
    };
    
    try {
      this.onMessage("input", handler);
      console.log("✓ Message handler 'input' registered successfully in onCreate");
    } catch (error) {
      console.error("✗ Failed to register message handler:", error);
    }
    
    // Start game loop
    this.startGameLoop();
  }

  onJoin(client: Client, options: any) {
    console.log(`Client ${client.sessionId} joined`);
    
    // Create player entity in ECS world
    this.world.createPlayer(client.sessionId);
    
    // Add player to state
    // Начальная позиция в центре мира (можно изменить позже)
    const player = new Player();
    player.id = client.sessionId;
    player.x = 0; // Начальная позиция в центре координат
    player.y = 0;
    player.vx = 0;
    player.vy = 0;
    player.direction = 0;
    this.state.players.set(client.sessionId, player);
  }

  onLeave(client: Client, consented: boolean) {
    console.log(`Client ${client.sessionId} left`);
    
    // Remove player from ECS world
    this.world.removePlayer(client.sessionId);
    
    // Remove player from state
    this.state.players.delete(client.sessionId);
  }


  onDispose() {
    console.log("GameRoom disposed");
    this.stopGameLoop();
  }

  private startGameLoop() {
    const tickDuration = 1000 / this.TICK_RATE;
    
    this.tickInterval = setInterval(() => {
      this.world.update(tickDuration / 1000); // Convert to seconds
      this.syncState();
    }, tickDuration);
  }

  private stopGameLoop() {
    if (this.tickInterval) {
      clearInterval(this.tickInterval);
      this.tickInterval = null;
    }
  }

  private syncState() {
    // Sync ECS world state to Colyseus state
    const entities = this.world.getReplicatedEntities();
    
    entities.forEach((entity, playerId) => {
      if (this.state.players.has(playerId)) {
        const player = this.state.players.get(playerId)!;
        const oldX = player.x;
        const oldY = player.y;
        player.x = entity.position.x;
        player.y = entity.position.y;
        player.vx = entity.velocity.x;
        player.vy = entity.velocity.y;
        player.direction = entity.direction.angle;
        
        // Log if position changed
        if (oldX !== player.x || oldY !== player.y) {
          console.log(`[syncState] Player ${playerId} moved: (${oldX.toFixed(2)}, ${oldY.toFixed(2)}) -> (${player.x.toFixed(2)}, ${player.y.toFixed(2)})`);
        }
      }
    });
  }
}

