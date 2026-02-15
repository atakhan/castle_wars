// Client-side schema matching server schema
// This is a simplified version for client-side type safety

export class Player {
  id: string = "";
  x: number = 0;
  y: number = 0;
  vx: number = 0;
  vy: number = 0;
  direction: number = 0; // угол в радианах
}

export class StaticEntity {
  id: string = "";
  x: number = 0;
  y: number = 0;
  shape: string = "aabb";
  radius: number = 0;
  halfWidth: number = 0;
  halfHeight: number = 0;
  interactableKind: string = "";
  interactableState: string = "";
}

export class GameState {
  players: Map<string, Player> = new Map();
  staticEntities: Map<string, StaticEntity> = new Map();
}

