#include "CollisionSystem.h"

void CollisionSystem::addEntity(Entity* e) { entities.push_back(e); }
void CollisionSystem::removeEntity(Entity* e) { 
    entities.erase(std::remove(entities.begin(), entities.end(), e), entities.end());
}

void CollisionSystem::update() {
    for (size_t i = 0; i < entities.size(); i++) {
        for (size_t j = i + 1; j < entities.size(); j++) {
            if (entities[i]->getHitBox().intersects(entities[j]->getHitBox())) {
                entities[i]->onCollision(entities[j]);
                entities[j]->onCollision(entities[i]);
            }
        }
    }
}
