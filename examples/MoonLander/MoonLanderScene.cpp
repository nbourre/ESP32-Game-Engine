/*
 * Implementation for Moonlander
 * 
 * Position, velocity tracked in meters and meters per second
 * Converted to pixel position when drawn
 */

#include "MoonLanderScene.h"
#include "EDGE.h"

const float gravity_force = 1.62;        // force in m/s^2
const float rocket_force = -2.5;         // force in m/s^2
const int screen_height_m = 100;         // displayed area in meters 
const float fuel_burn_sec = 1000;        // kg/s fuel burn
extern EDGE engine;

void MoonLanderScene::init() {
    screenHeight = engine.getRenderer().getHeight();
    screenWidth = engine.getRenderer().getWidth();
    elevation = screen_height_m - 12;   // start near top of screen
    velocity = 0.0;                     // initial velocity is zero
    fuel = 5000;                        // kg fuel starting
    started = false;                    // game not started
    done = false;                       // .. or complete
    success = false;                    // Not landed yet
    fire = false;                       // Engine not firing
    fire_toggle = false;
    end_delay_time = 0;
    pixels_per_m = ((float) screenHeight - 2) / screen_height_m;
}


void MoonLanderScene::update(unsigned long deltaTime_ms) {
    if (!started) {
        // Start game on button click
        if (engine.getInputManager().isButtonClicked(0)) {
            started = true;
        }
        return;
    }

    if (done) {
        if (end_delay_time < 500) {
            // Wait 1/2 second after completion before we acccept a button click.
            end_delay_time += deltaTime_ms;
            return;
        }
        if (engine.getInputManager().isButtonClicked(0)) {
            // Go back to ready state on button click.
            init();
        }
        return;
    }

    // Check if engine is currently firing
    if (engine.getInputManager().isButtonDown(0)) {
        if (fuel > 0) {
            velocity += (rocket_force * deltaTime_ms / 1000);
            fire = true;
            fire_toggle = !fire_toggle;
        } else {
            // Out of fuel. oops.
            fire = false;
        }
        // Calculate fuel consumption
        fuel -= fuel_burn_sec * deltaTime_ms / 1000;
        if (fuel < 0) {
            fuel = 0;
        }   
    } else {
        fire = false;
    }

    // Increase velocity due to gravity.
    velocity += (gravity_force * deltaTime_ms / 1000);

    // Elevation decreases with a positive velocity.
    elevation -= velocity;
    if (elevation <= 0) {
        elevation = 0;
        done = true;
        // Landing is OK if 0.5 m/s or less.
        if (velocity <= 0.5) {
            success = true;
        }
    }
}

void MoonLanderScene::draw(Renderer& renderer) {
    // location of bottom of lander on the screen in pixels
    int location = pixels_per_m * (screen_height_m - elevation);

    // Draw lunar lander body
    renderer.drawFilledRectangle(14, location - 9, 4, 2);
    renderer.drawFilledRectangle(12, location - 7, 8, 3);
    if (location >= 0) {
        // Draw lander legs. Don't pass a negative y value.
        renderer.drawLine(13, max(location - 4, 0), 10, location);
        renderer.drawLine(17, max(location - 4, 0), 20, location);
    }
    if (fire) {
        // Draw engine fire, toggling between 2 animations
        if (fire_toggle) {
            renderer.drawRectangle(14, location - 4, 1, 1);
            renderer.drawRectangle(16, location - 4, 1, 1);
            renderer.drawRectangle(15, location - 3, 1, 1);
            renderer.drawRectangle(14, location - 2, 1, 1);
            renderer.drawRectangle(16, location - 2, 1, 1);
            renderer.drawRectangle(15, location - 1, 1, 1);
        } else {
            renderer.drawRectangle(15, location - 4, 1, 1);
            renderer.drawRectangle(14, location - 3, 1, 1);
            renderer.drawRectangle(16, location - 3, 1, 1);
            renderer.drawRectangle(15, location - 2, 1, 1);
            renderer.drawRectangle(14, location - 1, 1, 1);
            renderer.drawRectangle(16, location - 1, 1, 1);
        }
    }

    // Draw the state information
    renderer.setFont(u8g2_font_6x10_tf);
    renderer.drawTextSafe(40, 22, "elev: %.2f m", elevation);
    renderer.drawTextSafe(40, 32, " vel: %.2f m/s", velocity);
    renderer.drawTextSafe(40, 42, "fuel: %.0f kg", fuel);
    renderer.drawLine(0, screenHeight - 1, screenWidth - 1, screenHeight - 1);

    // Draw game status if not running.
    if (!started) {
        renderer.drawText(30, 10, "READY");
    } else if (done) {
        if (success) {
            renderer.drawText(30, 10, "LANDED!");
        } else {
            renderer.drawText(30, 10, "CRASHED!");
        }
    }
}



