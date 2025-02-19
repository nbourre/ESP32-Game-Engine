/*
 * Scene for moon lander. 
 * Tracks elevation, velocity, fuel available. 
 * 
 * Using float is OK, ESP32 has an FPU
 * 
 * Consider tracking weight decrease as fuel os consumed.
 */

#ifndef MOON_LANDER_H
#define MOON_LANDER_H

#include "Scene.h"

// Size of area drawn into by the scene.
#define SCREEN_WIDTH 120
#define SCREEN_HEIGHT 60

class MoonLanderScene : public Scene {
public:
    void init() override;
    void update(unsigned long deltaTime_ms) override;
    void draw(Renderer& renderer) override;

private:
    float elevation;        // elevation in meters above the surface
    float velocity;         // lander velocity in m/s
    float fuel;             // remaing fuel in kg
    bool started;           // has the game started?
    bool done;              // is the game complete?
    bool success;           // landed ok?
    bool fire;              // engines firing
    bool fire_toggle;       // animate engine

    // Delay start after button is pushed.
    uint16_t start_delay_time;

    // Delay reset enabled after game done.
    uint16_t end_delay_time;
};

#endif
