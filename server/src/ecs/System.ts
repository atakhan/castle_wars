import { ComponentManager } from "./ComponentManager";

export abstract class System {
  protected componentManager!: ComponentManager;

  setComponentManager(componentManager: ComponentManager) {
    this.componentManager = componentManager;
  }

  abstract update(deltaTime: number): void;
}

