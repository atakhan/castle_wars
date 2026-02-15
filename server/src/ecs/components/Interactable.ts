export type InteractableKind = "chest" | "resource";
export type InteractableState = "closed" | "open" | "empty";

export class Interactable {
  constructor(
    public interactionRadius: number = 50,
    public kind: InteractableKind = "chest",
    public state: InteractableState = "closed"
  ) {}
}
