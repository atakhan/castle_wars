import { System } from "../System";
import { InputController } from "../components/InputController";
import { Velocity } from "../components/Velocity";
import { Direction } from "../components/Direction";

export class InputSystem extends System {
  private readonly MOVE_SPEED = 200; // pixels per second

  update(deltaTime: number): void {
    const entities = this.componentManager.getEntitiesWithComponent(InputController);

    for (const entityId of entities) {
      const input = this.componentManager.getComponent<InputController>(
        entityId,
        InputController
      );
      const velocity = this.componentManager.getComponent<Velocity>(entityId, Velocity);
      const direction = this.componentManager.getComponent<Direction>(entityId, Direction);

      if (input && velocity) {
        // Apply input to velocity
        const oldVx = velocity.x;
        const oldVy = velocity.y;
        velocity.x = input.moveX * this.MOVE_SPEED;
        velocity.y = input.moveY * this.MOVE_SPEED;
        
        // Update direction based on movement
        if (direction) {
          if (velocity.x !== 0 || velocity.y !== 0) {
            // Calculate angle from velocity vector
            direction.angle = Math.atan2(velocity.y, velocity.x);
          }
        }
        
        if (oldVx !== velocity.x || oldVy !== velocity.y) {
          console.log(`[InputSystem] Updated velocity for ${input.playerId}: (${oldVx.toFixed(2)}, ${oldVy.toFixed(2)}) -> (${velocity.x.toFixed(2)}, ${velocity.y.toFixed(2)})`);
        }
      }
    }
  }
}

