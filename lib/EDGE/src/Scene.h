#pragma once

class Scene {
public:
    virtual ~Scene() {}

    // Called once when the scene is set
    virtual void init() {}

    // Called in every update cycle with time elapsed in ms
    virtual void update(unsigned long deltaTime) = 0;

    // Called every frame to render the scene
    virtual void draw() = 0;

    // Called before switching away from the scene (if needed)
    virtual void exit() {}
};
