#pragma once
#include <Arduino.h>

// Define the number of buttons
#define BUTTON_COUNT 5 

//thinking about making a function to define button pins in your .ino file
//to make things easier cuz as is you have to come in here
//tho if i do that id want to do it for button count as well... 
//eh once i get images working

//Define pin numbers for up to 5 buttons
#define BUTTON_PIN_1 4      //select
#define BUTTON_PIN_2 16     //up
#define BUTTON_PIN_3 17     //down
#define BUTTON_PIN_4 5      //left
#define BUTTON_PIN_5 23     //right

class InputManager {
public:
    void init();
    void update(unsigned long dt);
    bool isButtonPressed(uint8_t buttonIndex);   // True on press for specific button
    bool isButtonReleased(uint8_t buttonIndex);  // True on release for specific button
    bool isButtonClicked(uint8_t buttonIndex);   // True on press and release for specific button
    bool isButtonDown(uint8_t buttonIndex);      // True while button is pressed for specific button

private:
    bool buttonState[BUTTON_COUNT];      // States of buttons (pressed or not)
    bool stateChanged[BUTTON_COUNT];     // Track state change for each button
    uint16_t waitTime[BUTTON_COUNT];     // Debounce time for each button
    bool clickFlag[BUTTON_COUNT];        // Used for isButtonClicked function for each button
    uint8_t buttonPins[BUTTON_COUNT];    // Array to store button pins
};