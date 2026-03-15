export interface Waypoint {
  x: number;
  y: number;
}

export class InputController {
  constructor(
    public playerId: string,
    public waypoints: Waypoint[] = []
  ) {}
}
