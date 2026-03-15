/**
 * Grid-based A* pathfinding. Obstacles are AABBs; they are expanded by playerRadius
 * so the path stays clear for a circular unit.
 */

export interface AABB {
  x: number;
  y: number;
  halfWidth: number;
  halfHeight: number;
}

export interface PathPoint {
  x: number;
  y: number;
}

const NEIGHBORS_8 = [
  [-1, -1], [-1, 0], [-1, 1],
  [0, -1],           [0, 1],
  [1, -1],  [1, 0],  [1, 1],
];

export class Pathfinder {
  private readonly cellSize: number;
  private readonly playerRadius: number;
  private readonly worldMinX: number;
  private readonly worldMinY: number;
  private readonly worldMaxX: number;
  private readonly worldMaxY: number;
  private gridWidth: number = 0;
  private gridHeight: number = 0;
  private walkable: boolean[] = [];

  constructor(
    cellSize: number = 24,
    playerRadius: number = 30,
    worldBounds: { minX: number; minY: number; maxX: number; maxY: number } = {
      minX: -450, minY: -450, maxX: 450, maxY: 450
    }
  ) {
    this.cellSize = cellSize;
    this.playerRadius = playerRadius;
    this.worldMinX = worldBounds.minX;
    this.worldMinY = worldBounds.minY;
    this.worldMaxX = worldBounds.maxX;
    this.worldMaxY = worldBounds.maxY;
    this.buildGridSize();
  }

  private buildGridSize() {
    this.gridWidth = Math.ceil((this.worldMaxX - this.worldMinX) / this.cellSize);
    this.gridHeight = Math.ceil((this.worldMaxY - this.worldMinY) / this.cellSize);
  }

  worldToGrid(x: number, y: number): [number, number] {
    const gx = Math.floor((x - this.worldMinX) / this.cellSize);
    const gy = Math.floor((y - this.worldMinY) / this.cellSize);
    return [gx, gy];
  }

  gridToWorld(gx: number, gy: number): [number, number] {
    const x = this.worldMinX + (gx + 0.5) * this.cellSize;
    const y = this.worldMinY + (gy + 0.5) * this.cellSize;
    return [x, y];
  }

  private cellCenter(gx: number, gy: number): { x: number; y: number } {
    const [x, y] = this.gridToWorld(gx, gy);
    return { x, y };
  }

  private isInBounds(gx: number, gy: number): boolean {
    return gx >= 0 && gx < this.gridWidth && gy >= 0 && gy < this.gridHeight;
  }

  private index(gx: number, gy: number): number {
    return gy * this.gridWidth + gx;
  }

  private pointInExpandedAABB(px: number, py: number, aabb: AABB, expand: number): boolean {
    const minX = aabb.x - aabb.halfWidth - expand;
    const maxX = aabb.x + aabb.halfWidth + expand;
    const minY = aabb.y - aabb.halfHeight - expand;
    const maxY = aabb.y + aabb.halfHeight + expand;
    return px >= minX && px <= maxX && py >= minY && py <= maxY;
  }

  buildWalkableGrid(obstacles: AABB[]): void {
    this.walkable = new Array(this.gridWidth * this.gridHeight).fill(true);
    for (let gy = 0; gy < this.gridHeight; gy++) {
      for (let gx = 0; gx < this.gridWidth; gx++) {
        const { x, y } = this.cellCenter(gx, gy);
        for (const obs of obstacles) {
          if (this.pointInExpandedAABB(x, y, obs, this.playerRadius)) {
            this.walkable[this.index(gx, gy)] = false;
            break;
          }
        }
      }
    }
  }

  findPath(startX: number, startY: number, endX: number, endY: number, obstacles: AABB[]): PathPoint[] {
    this.buildWalkableGrid(obstacles);

    const [startGx, startGy] = this.worldToGrid(startX, startY);
    const [endGx, endGy] = this.worldToGrid(endX, endY);

    if (!this.isInBounds(startGx, startGy) || !this.isInBounds(endGx, endGy)) {
      return [{ x: endX, y: endY }];
    }
    if (!this.walkable[this.index(startGx, startGy)]) {
      return [{ x: endX, y: endY }];
    }
    if (!this.walkable[this.index(endGx, endGy)]) {
      return [];
    }
    if (startGx === endGx && startGy === endGy) {
      return [{ x: endX, y: endY }];
    }

    const openSet = new Set<string>();
    const cameFrom = new Map<string, string>();
    const gScore = new Map<string, number>();
    const fScore = new Map<string, number>();

    const key = (gx: number, gy: number) => `${gx},${gy}`;
    const heuristic = (gx: number, gy: number) => {
      const dx = Math.abs(gx - endGx);
      const dy = Math.abs(gy - endGy);
      return Math.max(dx, dy) + (Math.SQRT2 - 1) * Math.min(dx, dy);
    };

    const startKey = key(startGx, startGy);
    openSet.add(startKey);
    gScore.set(startKey, 0);
    fScore.set(startKey, heuristic(startGx, startGy));

    while (openSet.size > 0) {
      let currentKey = "";
      let bestF = Infinity;
      for (const k of openSet) {
        const f = fScore.get(k) ?? Infinity;
        if (f < bestF) {
          bestF = f;
          currentKey = k;
        }
      }
      if (!currentKey) break;

      const [cx, cy] = currentKey.split(",").map(Number);
      if (cx === endGx && cy === endGy) {
        const path: PathPoint[] = [];
        let c: string | undefined = currentKey;
        while (c) {
          const [gx, gy] = c.split(",").map(Number);
          const { x, y } = this.cellCenter(gx, gy);
          path.unshift({ x, y });
          c = cameFrom.get(c);
        }
        path[path.length - 1] = { x: endX, y: endY };
        return path;
      }

      openSet.delete(currentKey);

      for (const [dx, dy] of NEIGHBORS_8) {
        const nx = cx + dx;
        const ny = cy + dy;
        if (!this.isInBounds(nx, ny)) continue;
        if (!this.walkable[this.index(nx, ny)]) continue;

        const moveCost = dx !== 0 && dy !== 0 ? Math.SQRT2 : 1;
        const neighborKey = key(nx, ny);
        const tentativeG = (gScore.get(currentKey) ?? Infinity) + moveCost;

        if (tentativeG < (gScore.get(neighborKey) ?? Infinity)) {
          cameFrom.set(neighborKey, currentKey);
          gScore.set(neighborKey, tentativeG);
          fScore.set(neighborKey, tentativeG + heuristic(nx, ny));
          openSet.add(neighborKey);
        }
      }
    }

    return [{ x: endX, y: endY }];
  }
}
