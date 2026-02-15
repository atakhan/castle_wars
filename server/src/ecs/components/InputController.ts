export class InputController {
  constructor(
    public playerId: string,
    public targetX: number | null = null,
    public targetY: number | null = null
  ) {}
}
