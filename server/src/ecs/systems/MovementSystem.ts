import { System } from "../System";
import { Position } from "../components/Position";
import { Velocity } from "../components/Velocity";

export class MovementSystem extends System {
  update(deltaTime: number): void {
    const entities = this.componentManager.getEntitiesWithComponent(Position);

    for (const entityId of entities) {
      const position = this.componentManager.getComponent<Position>(entityId, Position);
      const velocity = this.componentManager.getComponent<Velocity>(entityId, Velocity);

      if (position && velocity) {
        // Update position based on velocity
        const oldX = position.x;
        const oldY = position.y;
        position.x += velocity.x * deltaTime;
        position.y += velocity.y * deltaTime;
        
        if (oldX !== position.x || oldY !== position.y) {
          console.log(`[MovementSystem] Updated position: (${oldX.toFixed(2)}, ${oldY.toFixed(2)}) -> (${position.x.toFixed(2)}, ${position.y.toFixed(2)}), velocity: (${velocity.x.toFixed(2)}, ${velocity.y.toFixed(2)}), deltaTime: ${deltaTime.toFixed(4)}`);
        }
      }
    }
  }
}

