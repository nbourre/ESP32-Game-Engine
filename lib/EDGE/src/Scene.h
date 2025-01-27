#pragma once
#include <ArduinoQueue.h>
#include "Entity.h"

#define MAX_ENTITIES 10  // Adjustable max entities per scene

class Scene {
public:
    virtual ~Scene() {}

    virtual void init() {}
    virtual void update(unsigned long deltaTime);
    virtual void draw(Renderer& renderer);

    void addEntity(Entity* entity);
    void removeEntity(Entity* entity);

protected:
    ArduinoQueue<Entity*> entities;  // Safe preallocated queue
};
