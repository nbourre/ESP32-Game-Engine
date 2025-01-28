#pragma once
#include <U8g2lib.h>
#include <SafeString.h>

#define I2C_SDA 5 // Compatible with ESP32-C3 with OLED
#define I2C_SCL 6 // Compatible with ESP32-C3 with OLED

class Renderer {
public:
    Renderer(): u8g2(U8G2_R0, U8X8_PIN_NONE, I2C_SCL, I2C_SDA) {}
    Renderer(int resetPin, int sdaPin, int sclPin) : u8g2(U8G2_R0, resetPin, sdaPin, sclPin) {}

    Renderer(uint8_t address, int resetPin, int sdaPin, int sclPin) : u8g2(U8G2_R0, resetPin, sdaPin, sclPin) {
        u8g2.setI2CAddress(address);
    }

    void setI2CAddress(uint8_t address) {
        u8g2.setI2CAddress(address);
    }

    void init();
    void beginFrame();
    void endFrame();
    
    
    void drawText(int x, int y, const char* str);
    void drawTextSafe(int x, int y, const char* format, ...);
    void drawCircle(int x, int y, int radius);
    void drawRectangle(int x, int y, int width, int height);
    void drawFilledRectangle(int x, int y, int width, int height);
    void drawLine(int x1, int y1, int x2, int y2);

    void setDisplayBufferSize(int w, int h) {
        displayBufferWidth = w;
        displayBufferHeight = h;        
    }

    void setDisplaySize(int w, int h) {
        width = w;
        height = h;
    }

    void setDisplayOffset(int x, int y) {
        xOffset = x;
        yOffset = y;
    }

    void setContrast(uint8_t level) {
        u8g2.setContrast(level);
    }

    void setFont(const uint8_t* font);



private:
    U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;

    // Default values for SSD1306 128x64 display
    int displayBufferWidth = 132;
    int displayBufferHeight = 64;

    // Default values for OLED 0.42" display
    int width = 72;
    int height = 40;

    // Offset to center the display in the buffer
    int xOffset = (displayBufferWidth - width) / 2;
    int yOffset = (displayBufferHeight - height) / 2;
};
