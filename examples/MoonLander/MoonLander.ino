/*
 * Moon Lander - use the rocket to land at 5 m/s or less.
 */

#include <Arduino.h>
#include "EDGE.h"
#include "MoonLanderScene.h"

#define I2C_SDA SDA
#define I2C_SCL SCL

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// OLED Display
// SCREEN_WIDTH and SCREEN_HEIGHT defined in MoonLanderScene.h
DisplayConfig config(SSD1306, U8G2_R0, I2C_SCL, I2C_SDA, U8X8_PIN_NONE, U8X8_PIN_NONE, U8X8_PIN_NONE, SCREEN_WIDTH, SCREEN_HEIGHT, true);
InputConfig inputConfig(1, 0); // One button connected to pin 0

EDGE engine(config, inputConfig);
MoonLanderScene landerScene;

void setup() {
    // Useful if we want to add debug messages
    Serial.begin(115200);
    delay(100);

    engine.init();
    engine.setScene(&landerScene);
}

void loop() {
    engine.update();
    engine.draw();
}

