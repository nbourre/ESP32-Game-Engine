#include "Scene.h"

void Scene::update(unsigned long deltaTime) {
    int count = entities.itemCount();
    for (int i = 0; i < count; i++) {
        Entity* entity = entities.dequeue();  // Get entity from queue
        entity->update(deltaTime);
        entities.enqueue(entity);  // Re-add entity to maintain order
    }
}

void Scene::draw(Renderer& renderer) {
    int count = entities.itemCount();
    for (int i = 0; i < count; i++) {
        Entity* entity = entities.dequeue();  // Get entity from queue
        entity->draw(renderer);
        entities.enqueue(entity);  // Re-add entity to maintain order
    }
}

void Scene::addEntity(Entity* entity) {
    entities.enqueue(entity);
}

void Scene::removeEntity(Entity* entity) {
    ArduinoQueue<Entity*> tempQueue; 
    while (!entities.isEmpty()) {
        Entity* e = entities.dequeue();
        if (e != entity) {
            tempQueue.enqueue(e);  // Keep all except the one to remove
        }
    }
    entities = tempQueue;
}
