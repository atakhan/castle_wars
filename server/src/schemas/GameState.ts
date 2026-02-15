import { Schema, MapSchema, type } from "@colyseus/schema";

export class Player extends Schema {
  @type("string") id: string = "";
  @type("number") x: number = 0;
  @type("number") y: number = 0;
  @type("number") vx: number = 0;
  @type("number") vy: number = 0;
  @type("number") direction: number = 0; // угол в радианах
}

export class StaticEntity extends Schema {
  @type("string") id: string = "";
  @type("number") x: number = 0;
  @type("number") y: number = 0;
  @type("string") shape: string = "aabb"; // "circle" | "aabb"
  @type("number") radius: number = 0;
  @type("number") halfWidth: number = 0;
  @type("number") halfHeight: number = 0;
  @type("string") interactableKind: string = ""; // "" | "chest" | "resource"
  @type("string") interactableState: string = ""; // "" | "closed" | "open" | "empty"
}

export class GameState extends Schema {
  @type({ map: Player })
  players = new MapSchema<Player>();
  @type({ map: StaticEntity })
  staticEntities = new MapSchema<StaticEntity>();
}

