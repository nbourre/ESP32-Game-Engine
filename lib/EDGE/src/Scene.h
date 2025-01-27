#pragma once
#include "Entity.h"

#define MAX_ENTITIES 50  // Adjust based on needs

class Scene {
public:
    virtual ~Scene() {}

    virtual void init() {}
    virtual void update(unsigned long deltaTime);
    virtual void draw(Renderer& renderer);

    void addEntity(Entity* entity);
    void removeEntity(Entity* entity);

protected:
    Entity* entities[MAX_ENTITIES] = {nullptr};  // Fixed-size array
    int entityCount = 0;
};
