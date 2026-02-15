export class ComponentManager {
  private components: Map<string, Map<Function, any>> = new Map();

  addComponent<T>(entityId: string, component: T) {
    if (!this.components.has(entityId)) {
      this.components.set(entityId, new Map());
    }
    
    const entityComponents = this.components.get(entityId)!;
    entityComponents.set(component.constructor, component);
  }

  getComponent<T>(entityId: string, componentClass: new (...args: any[]) => T): T | null {
    const entityComponents = this.components.get(entityId);
    if (!entityComponents) return null;
    
    return (entityComponents.get(componentClass) as T) || null;
  }

  removeComponent(entityId: string, componentClass: Function) {
    const entityComponents = this.components.get(entityId);
    if (entityComponents) {
      entityComponents.delete(componentClass);
    }
  }

  removeEntity(entityId: string) {
    this.components.delete(entityId);
  }

  getEntitiesWithComponent<T>(componentClass: new (...args: any[]) => T): string[] {
    const entities: string[] = [];
    
    for (const [entityId, components] of this.components.entries()) {
      if (components.has(componentClass)) {
        entities.push(entityId);
      }
    }
    
    return entities;
  }
}

