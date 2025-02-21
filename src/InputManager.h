#pragma once
#include <Arduino.h>

#define BUTTON_PIN 9  // Adjust this for your hardware

class InputManager {
public:
    void init();
    void update(unsigned long dt);
    bool isButtonPressed();     // True on press
    bool isButtonReleased();    // True on release
    bool isButtonClicked();     // True on press and release 
    bool isButtonDown();        // True while button is pressed

private:
    bool buttonState;
    bool stateChanged;          // True if state changed in update
    uint16_t waitTime;          // Time (ms) remaining before reading buttton
    bool clickFlag;             // Used for isButtonClicked function
};
