export type ColliderShape = "circle" | "aabb";

export class Collider {
  constructor(
    public shape: ColliderShape,
    public isStatic: boolean,
    // For circle:
    public radius: number = 0,
    // For aabb (half-extents, centered at position):
    public halfWidth: number = 0,
    public halfHeight: number = 0
  ) {}
}
