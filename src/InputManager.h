#pragma once
#include <Arduino.h>

#define BUTTON_PIN 9  // Adjust this for your hardware

class InputManager {
public:
    void init();
    void update(unsigned long dt);
    bool isButtonPressed();
    bool isButtonDown();

private:
    bool buttonState;
    bool lastButtonState;
};
