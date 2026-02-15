import { System } from "../System";
import { InputController } from "../components/InputController";
import { Velocity } from "../components/Velocity";
import { Direction } from "../components/Direction";
import { Position } from "../components/Position";

export class InputSystem extends System {
  private readonly MOVE_SPEED = 200; // pixels per second
  private readonly ARRIVAL_RADIUS = 15; // circle of arrival - inside we slow down
  private readonly SNAP_THRESHOLD = 1; // snap to target and stop when closer than this

  update(deltaTime: number): void {
    const entities = this.componentManager.getEntitiesWithComponent(InputController);

    for (const entityId of entities) {
      const input = this.componentManager.getComponent<InputController>(
        entityId,
        InputController
      );
      const position = this.componentManager.getComponent<Position>(entityId, Position);
      const velocity = this.componentManager.getComponent<Velocity>(entityId, Velocity);
      const direction = this.componentManager.getComponent<Direction>(entityId, Direction);

      if (!input || !velocity || !position) continue;

      if (input.targetX !== null && input.targetY !== null) {
        const dx = input.targetX - position.x;
        const dy = input.targetY - position.y;
        const dist = Math.sqrt(dx * dx + dy * dy);

        if (dist < this.SNAP_THRESHOLD) {
          position.x = input.targetX;
          position.y = input.targetY;
          velocity.x = 0;
          velocity.y = 0;
          input.targetX = null;
          input.targetY = null;
        } else if (dist <= this.ARRIVAL_RADIUS) {
          const nx = dx / dist;
          const ny = dy / dist;
          const speed = this.MOVE_SPEED * (dist / this.ARRIVAL_RADIUS);
          velocity.x = nx * speed;
          velocity.y = ny * speed;
          if (direction) {
            direction.angle = Math.atan2(velocity.y, velocity.x);
          }
        } else {
          const nx = dx / dist;
          const ny = dy / dist;
          velocity.x = nx * this.MOVE_SPEED;
          velocity.y = ny * this.MOVE_SPEED;
          if (direction) {
            direction.angle = Math.atan2(velocity.y, velocity.x);
          }
        }
      } else {
        velocity.x = 0;
        velocity.y = 0;
      }
    }
  }
}
