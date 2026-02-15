import { ComponentManager } from "./ComponentManager";
import { System } from "./System";

export class SystemManager {
  private systems: System[] = [];

  constructor(private componentManager: ComponentManager) {}

  registerSystem(system: System) {
    system.setComponentManager(this.componentManager);
    this.systems.push(system);
  }

  update(deltaTime: number) {
    for (const system of this.systems) {
      system.update(deltaTime);
    }
  }
}

