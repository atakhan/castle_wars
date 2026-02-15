import { Entity } from "./Entity";
import { ComponentManager } from "./ComponentManager";
import { SystemManager } from "./SystemManager";
import { Position } from "./components/Position";
import { Velocity } from "./components/Velocity";
import { InputController } from "./components/InputController";
import { NetworkReplication } from "./components/NetworkReplication";
import { Direction } from "./components/Direction";
import { MovementSystem } from "./systems/MovementSystem";
import { InputSystem } from "./systems/InputSystem";

export class GameWorld {
  private entities: Map<string, Entity> = new Map();
  private componentManager: ComponentManager;
  private systemManager: SystemManager;
  private nextEntityId: number = 0;

  constructor() {
    this.componentManager = new ComponentManager();
    this.systemManager = new SystemManager(this.componentManager);
    
    // Register systems
    this.systemManager.registerSystem(new InputSystem());
    this.systemManager.registerSystem(new MovementSystem());
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

  handlePlayerInput(playerId: string, input: any) {
    // Find entity by playerId
    let found = false;
    for (const [entityId] of this.entities.entries()) {
      const inputController = this.componentManager.getComponent<InputController>(
        entityId,
        InputController
      );
      
      if (inputController && inputController.playerId === playerId) {
        inputController.moveX = input.moveX || 0;
        inputController.moveY = input.moveY || 0;
        found = true;
        console.log(`[GameWorld] Updated input for player ${playerId}: moveX=${inputController.moveX}, moveY=${inputController.moveY}`);
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

