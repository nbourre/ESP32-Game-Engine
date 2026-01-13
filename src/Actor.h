#pragma once
#include "Entity.h"
#include <CollisionTypes.h>

class Actor : public Entity {
public:
    Actor(float x, float y, int w, int h) : Entity(x, y, w, h, EntityType::ACTOR) {}
    virtual ~Actor() = default;

    CollisionLayer layer = DefaultLayers::kNone;
    CollisionLayer mask  = DefaultLayers::kNone;

    void setCollisionLayer(CollisionLayer l) { layer = l; }
    void setCollisionMask(CollisionLayer m)  { mask = m; }

    void update(unsigned long dt) override {

    }

    bool isInLayer(uint16_t targetLayer) const {
        return (layer & targetLayer) != 0;
    }

    virtual Rect getHitBox() = 0;
    virtual void onCollision(Actor* other) = 0;
};