#include "Scene.h"

void Scene::update(unsigned long deltaTime) {
    for (int i = 0; i < entityCount; i++) {
        if (entities[i]) {
            entities[i]->update(deltaTime);
        }
    }
}

void Scene::draw(Renderer& renderer) {
    for (int i = 0; i < entityCount; i++) {
        if (entities[i]) {
            entities[i]->draw(renderer);
        }
    }
}

void Scene::addEntity(Entity* entity) {
    if (entityCount < MAX_ENTITIES) {
        entities[entityCount++] = entity;
    }
}

void Scene::removeEntity(Entity* entity) {
    for (int i = 0; i < entityCount; i++) {
        if (entities[i] == entity) {
            entities[i] = nullptr;
            // Shift remaining entities left
            for (int j = i; j < entityCount - 1; j++) {
                entities[j] = entities[j + 1];
            }
            entityCount--;
            return;
        }
    }
}
