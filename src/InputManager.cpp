#include "InputManager.h"

void InputManager::init() {
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    buttonState = false;
    lastButtonState = false;
}

void InputManager::update(unsigned long dt) {
    bool reading = digitalRead(BUTTON_PIN) == LOW;
    
    if (reading && !lastButtonState) {
        buttonState = true; // Button just pressed
    } else {
        buttonState = false;
    }

    lastButtonState = reading;
}

bool InputManager::isButtonPressed() {
    return buttonState;
}

bool InputManager::isButtonDown() {
    return lastButtonState;
}
