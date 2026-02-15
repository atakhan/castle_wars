import { System } from "../System";
import { Position } from "../components/Position";
import { Velocity } from "../components/Velocity";
import { Collider } from "../components/Collider";

export class CollisionSystem extends System {
  update(_deltaTime: number): void {
    const entities = this.componentManager.getEntitiesWithComponent(Collider);
    const dynamicEntries: { entityId: string; position: Position; velocity: Velocity; collider: Collider }[] = [];
    const staticEntries: { entityId: string; position: Position; collider: Collider }[] = [];

    for (const entityId of entities) {
      const position = this.componentManager.getComponent<Position>(entityId, Position);
      const collider = this.componentManager.getComponent<Collider>(entityId, Collider);
      if (!position || !collider) continue;

      if (collider.isStatic) {
        staticEntries.push({ entityId, position, collider });
      } else {
        const velocity = this.componentManager.getComponent<Velocity>(entityId, Velocity);
        if (velocity) {
          dynamicEntries.push({ entityId, position, velocity, collider });
        }
      }
    }

    for (const dynamic of dynamicEntries) {
      for (const staticEntry of staticEntries) {
        this.resolveCollision(dynamic.position, dynamic.collider, staticEntry.position, staticEntry.collider);
      }
    }
  }

  private resolveCollision(
    posA: Position,
    colA: Collider,
    posB: Position,
    colB: Collider
  ): void {
    if (colA.shape === "circle" && colB.shape === "circle") {
      this.resolveCircleVsCircle(posA, colA.radius, posB, colB.radius);
    } else if (colA.shape === "circle" && colB.shape === "aabb") {
      this.resolveCircleVsAabb(posA, colA.radius, posB, colB.halfWidth, colB.halfHeight);
    } else if (colA.shape === "aabb" && colB.shape === "circle") {
      this.resolveAabbVsCircle(posA, colA.halfWidth, colA.halfHeight, posB, colB.radius);
    } else if (colA.shape === "aabb" && colB.shape === "aabb") {
      this.resolveAabbVsAabb(posA, colA.halfWidth, colA.halfHeight, posB, colB.halfWidth, colB.halfHeight);
    }
  }

  private resolveCircleVsCircle(posA: Position, rA: number, posB: Position, rB: number): void {
    const dx = posB.x - posA.x;
    const dy = posB.y - posA.y;
    const dist = Math.sqrt(dx * dx + dy * dy);
    const overlap = rA + rB - dist;
    if (overlap <= 0 || dist < 1e-6) return;

    const nx = dx / dist;
    const ny = dy / dist;
    posA.x -= nx * overlap;
    posA.y -= ny * overlap;
  }

  private resolveCircleVsAabb(
    circlePos: Position,
    radius: number,
    aabbPos: Position,
    halfWidth: number,
    halfHeight: number
  ): void {
    const closestX = Math.max(aabbPos.x - halfWidth, Math.min(circlePos.x, aabbPos.x + halfWidth));
    const closestY = Math.max(aabbPos.y - halfHeight, Math.min(circlePos.y, aabbPos.y + halfHeight));
    const dx = circlePos.x - closestX;
    const dy = circlePos.y - closestY;
    const distSq = dx * dx + dy * dy;
    const dist = Math.sqrt(distSq);
    if (dist < 1e-6) {
      circlePos.x = aabbPos.x + halfWidth + radius;
      return;
    }
    const overlap = radius - dist;
    if (overlap <= 0) return;

    const nx = dx / dist;
    const ny = dy / dist;
    circlePos.x += nx * overlap;
    circlePos.y += ny * overlap;
  }

  private resolveAabbVsCircle(
    aabbPos: Position,
    halfWidth: number,
    halfHeight: number,
    circlePos: Position,
    radius: number
  ): void {
    const closestX = Math.max(aabbPos.x - halfWidth, Math.min(circlePos.x, aabbPos.x + halfWidth));
    const closestY = Math.max(aabbPos.y - halfHeight, Math.min(circlePos.y, aabbPos.y + halfHeight));
    const dx = circlePos.x - closestX;
    const dy = circlePos.y - closestY;
    const dist = Math.sqrt(dx * dx + dy * dy);
    const overlap = radius - dist;
    if (overlap <= 0 || dist < 1e-6) return;
    const nx = dx / dist;
    const ny = dy / dist;
    aabbPos.x += nx * overlap;
    aabbPos.y += ny * overlap;
  }

  private resolveAabbVsAabb(
    posA: Position,
    hwA: number,
    hhA: number,
    posB: Position,
    hwB: number,
    hhB: number
  ): void {
    const leftA = posA.x - hwA;
    const rightA = posA.x + hwA;
    const topA = posA.y - hhA;
    const bottomA = posA.y + hhA;
    const leftB = posB.x - hwB;
    const rightB = posB.x + hwB;
    const topB = posB.y - hhB;
    const bottomB = posB.y + hhB;

    const overlapLeft = rightA - leftB;
    const overlapRight = rightB - leftA;
    const overlapTop = bottomA - topB;
    const overlapBottom = bottomB - topA;

    if (overlapLeft <= 0 || overlapRight <= 0 || overlapTop <= 0 || overlapBottom <= 0) return;

    const minOverlapX = Math.min(overlapLeft, overlapRight);
    const minOverlapY = Math.min(overlapTop, overlapBottom);
    if (minOverlapX < minOverlapY) {
      posA.x += overlapLeft < overlapRight ? -minOverlapX : minOverlapX;
    } else {
      posA.y += overlapTop < overlapBottom ? -minOverlapY : minOverlapY;
    }
  }
}
