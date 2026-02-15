import { Entity } from "./Entity";
import { ComponentManager } from "./ComponentManager";
import { SystemManager } from "./SystemManager";
import { Position } from "./components/Position";
import { Velocity } from "./components/Velocity";
import { InputController } from "./components/InputController";
import { NetworkReplication } from "./components/NetworkReplication";
import { Direction } from "./components/Direction";
import { Collider } from "./components/Collider";
import { Interactable } from "./components/Interactable";
import { MovementSystem } from "./systems/MovementSystem";
import { InputSystem } from "./systems/InputSystem";
import { CollisionSystem } from "./systems/CollisionSystem";

export class GameWorld {
  private entities: Map<string, Entity> = new Map();
  private staticEntityIds: Set<string> = new Set();
  private componentManager: ComponentManager;
  private systemManager: SystemManager;
  private nextEntityId: number = 0;

  constructor() {
    this.componentManager = new ComponentManager();
    this.systemManager = new SystemManager(this.componentManager);
    
    // Register systems (order: input -> movement -> collision)
    this.systemManager.registerSystem(new InputSystem());
    this.systemManager.registerSystem(new MovementSystem());
    this.systemManager.registerSystem(new CollisionSystem());
  }

  createPlayer(playerId: string): string {
    const entityId = `player_${this.nextEntityId++}`;
    const entity = new Entity(entityId);
    
    // Add components
    this.componentManager.addComponent(entityId, new Position(0, 0));
    this.componentManager.addComponent(entityId, new Velocity(0, 0));
    this.componentManager.addComponent(entityId, new InputController(playerId));
    this.componentManager.addComponent(entityId, new NetworkReplication(playerId));
    this.componentManager.addComponent(entityId, new Direction(0));
    this.componentManager.addComponent(entityId, new Collider("circle", false, 30, 0, 0));
    
    this.entities.set(entityId, entity);
    return entityId;
  }

  removePlayer(playerId: string) {
    // Find entity by playerId
    for (const [entityId, entity] of this.entities.entries()) {
      const inputController = this.componentManager.getComponent<InputController>(
        entityId,
        InputController
      );
      
      if (inputController && inputController.playerId === playerId) {
        this.componentManager.removeEntity(entityId);
        this.entities.delete(entityId);
        break;
      }
    }
  }

  private lastMoveToTime: Map<string, number> = new Map();
  private readonly MOVE_TO_RATE_MS = 100;

  handlePlayerInput(playerId: string, input: any) {
    const targetX = typeof input.targetX === "number" ? input.targetX : null;
    const targetY = typeof input.targetY === "number" ? input.targetY : null;
    if (targetX === null || targetY === null) return;

    const now = Date.now();
    const last = this.lastMoveToTime.get(playerId) ?? 0;
    if (now - last < this.MOVE_TO_RATE_MS) return;
    this.lastMoveToTime.set(playerId, now);

    let found = false;
    for (const [entityId] of this.entities.entries()) {
      const inputController = this.componentManager.getComponent<InputController>(
        entityId,
        InputController
      );

      if (inputController && inputController.playerId === playerId) {
        inputController.targetX = targetX;
        inputController.targetY = targetY;
        found = true;
        console.log(`[GameWorld] Updated target for player ${playerId}: (${targetX.toFixed(2)}, ${targetY.toFixed(2)})`);
        break;
      }
    }

    if (!found) {
      console.warn(`[GameWorld] Player ${playerId} not found in entities! Total entities: ${this.entities.size}`);
    }
  }

  update(deltaTime: number) {
    this.systemManager.update(deltaTime);
  }

  spawnStaticObstacles(): void {
    // Walls / boxes - AABB obstacles
    const obstacles: { x: number; y: number; halfWidth: number; halfHeight: number }[] = [
      { x: 200, y: 0, halfWidth: 80, halfHeight: 20 },
      { x: -200, y: 100, halfWidth: 60, halfHeight: 100 },
      { x: 150, y: -150, halfWidth: 40, halfHeight: 40 },
      { x: -100, y: -200, halfWidth: 120, halfHeight: 25 },
    ];
    for (let i = 0; i < obstacles.length; i++) {
      const o = obstacles[i];
      const entityId = `static_${this.nextEntityId++}`;
      const entity = new Entity(entityId);
      this.componentManager.addComponent(entityId, new Position(o.x, o.y));
      this.componentManager.addComponent(
        entityId,
        new Collider("aabb", true, 0, o.halfWidth, o.halfHeight)
      );
      this.entities.set(entityId, entity);
      this.staticEntityIds.add(entityId);
    }
  }

  spawnInteractables(): void {
    const items: { x: number; y: number; halfWidth: number; halfHeight: number; kind: "chest" | "resource" }[] = [
      { x: 0, y: -100, halfWidth: 25, halfHeight: 25, kind: "chest" },
      { x: 180, y: -80, halfWidth: 20, halfHeight: 20, kind: "resource" },
    ];
    for (const item of items) {
      const entityId = `static_${this.nextEntityId++}`;
      const entity = new Entity(entityId);
      this.componentManager.addComponent(entityId, new Position(item.x, item.y));
      this.componentManager.addComponent(
        entityId,
        new Collider("aabb", true, 0, item.halfWidth, item.halfHeight)
      );
      this.componentManager.addComponent(
        entityId,
        new Interactable(60, item.kind, "closed")
      );
      this.entities.set(entityId, entity);
      this.staticEntityIds.add(entityId);
    }
  }

  getStaticEntities(): Map<string, { position: Position; collider: Collider; interactable?: Interactable }> {
    const result = new Map<string, { position: Position; collider: Collider; interactable?: Interactable }>();
    for (const entityId of this.staticEntityIds) {
      const position = this.componentManager.getComponent<Position>(entityId, Position);
      const collider = this.componentManager.getComponent<Collider>(entityId, Collider);
      const interactable = this.componentManager.getComponent<Interactable>(entityId, Interactable);
      if (position && collider) {
        result.set(entityId, { position, collider, interactable: interactable ?? undefined });
      }
    }
    return result;
  }

  handleInteract(playerId: string): { entityId: string; newState: string } | null {
    let playerPosition: Position | null = null;
    for (const [entityId] of this.entities.entries()) {
      const inputController = this.componentManager.getComponent<InputController>(entityId, InputController);
      if (inputController && inputController.playerId === playerId) {
        playerPosition = this.componentManager.getComponent<Position>(entityId, Position);
        break;
      }
    }
    if (!playerPosition) return null;

    let closest: { entityId: string; interactable: Interactable; distSq: number } | null = null;
    for (const entityId of this.staticEntityIds) {
      const interactable = this.componentManager.getComponent<Interactable>(entityId, Interactable);
      const position = this.componentManager.getComponent<Position>(entityId, Position);
      if (!interactable || !position || interactable.state !== "closed") continue;

      const dx = position.x - playerPosition.x;
      const dy = position.y - playerPosition.y;
      const distSq = dx * dx + dy * dy;
      const maxDist = interactable.interactionRadius;
      if (distSq <= maxDist * maxDist && (!closest || distSq < closest.distSq)) {
        closest = { entityId, interactable, distSq };
      }
    }
    if (!closest) return null;

    closest.interactable.state = "open";
    return { entityId: closest.entityId, newState: "open" };
  }

  getReplicatedEntities(): Map<string, { position: Position; velocity: Velocity; direction: Direction }> {
    const result = new Map();
    
    for (const [entityId] of this.entities.entries()) {
      const replication = this.componentManager.getComponent<NetworkReplication>(
        entityId,
        NetworkReplication
      );
      
      if (replication && replication.shouldReplicate) {
        const position = this.componentManager.getComponent<Position>(entityId, Position);
        const velocity = this.componentManager.getComponent<Velocity>(entityId, Velocity);
        const direction = this.componentManager.getComponent<Direction>(entityId, Direction);
        
        if (position && velocity && direction) {
          result.set(replication.playerId, { position, velocity, direction });
        }
      }
    }
    
    return result;
  }
}

