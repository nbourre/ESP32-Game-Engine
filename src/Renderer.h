#pragma once
#include <U8g2lib.h>
#include "DisplayConfig.h"
#include <SafeString.h>

class Renderer {
public:
    Renderer(const DisplayConfig& config);
    void init();
    void beginFrame();
    void endFrame();

    // Return a pointer to the U8G2 object
    U8G2* getU8G2() { return u8g2; }
    
    void drawText(int x, int y, const char* str);
    void drawTextSafe(int x, int y, const char* format, ...);
    void drawCircle(int x, int y, int radius);
    void drawRectangle(int x, int y, int width, int height);
    void drawFilledRectangle(int x, int y, int width, int height);
    void drawLine(int x1, int y1, int x2, int y2);

    void setDisplaySize(int w, int h) {
        width = w;
        height = h;
    }

    void setDisplayOffset(int x, int y) {
        xOffset = x;
        yOffset = y;
    }

    void setContrast(uint8_t level) {
        u8g2->setContrast(level);
    }

    void setFont(const uint8_t* font);
    void setFontSize(int size);

    int getXOffset() const { return xOffset; }
    int getYOffset() const { return yOffset; }



private:
    U8G2* u8g2;
    DisplayConfig config;

    int width = 128;
    int height = 64;

    int xOffset = 0;
    int yOffset = 0;
};
