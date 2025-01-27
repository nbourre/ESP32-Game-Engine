#include <Arduino.h>
#include "EDGE.h"
#include "FlappyBirdScene.h"

EDGE engine;
FlappyBirdScene scene;

unsigned long currentTime = 0;

void printTask(ulong time);

void setup() {
    Serial.begin(115200);
    engine.init();
    engine.setScene(&scene);

    Serial.println("Setup completed");
}

void loop() {
    currentTime = millis();
    engine.update();
    engine.draw();
}
