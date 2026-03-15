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

      const target = input.waypoints.length > 0 ? input.waypoints[0] : null;

      if (target) {
        const dx = target.x - position.x;
        const dy = target.y - position.y;
        const dist = Math.sqrt(dx * dx + dy * dy);

        if (dist < this.SNAP_THRESHOLD) {
          position.x = target.x;
          position.y = target.y;
          velocity.x = 0;
          velocity.y = 0;
          input.waypoints.shift();
          if (input.waypoints.length === 0) continue;
          const next = input.waypoints[0];
          const ndx = next.x - position.x;
          const ndy = next.y - position.y;
          const ndist = Math.sqrt(ndx * ndx + ndy * ndy);
          if (ndist >= this.SNAP_THRESHOLD) {
            const nx = ndx / ndist;
            const ny = ndy / ndist;
            const speed = ndist <= this.ARRIVAL_RADIUS
              ? this.MOVE_SPEED * (ndist / this.ARRIVAL_RADIUS)
              : this.MOVE_SPEED;
            velocity.x = nx * speed;
            velocity.y = ny * speed;
            if (direction) direction.angle = Math.atan2(velocity.y, velocity.x);
          }
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
