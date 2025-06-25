#pragma once
#include <U8g2lib.h>
#include "DisplayConfig.h" 
#include <functional> // For std::function

// Define the logger type directly to break circular dependency with EDGE.h
using EDGELogger = std::function<void(const char* message)>;

class Renderer {
public:
    Renderer(U8G2* u8g2_ptr, const DisplayConfig& displayConf);
    void init();
    void beginFrame();
    void endFrame();

    U8G2* getU8G2() { return u8g2; }
    void setLogger(EDGELogger logger) { _logger = logger; }


    void drawText(int x, int y, const char* str);
    void drawTextSafe(int x, int y, const char* format, ...);
    void drawCircle(int x, int y, int radius);
    void drawFilledCircle(int x, int y, int radius);
    void drawRectangle(int x, int y, int width, int height);
    void drawFilledRectangle(int x, int y, int width, int height);
    void drawLine(int x1, int y1, int x2, int y2);

    void setContrast(uint8_t level);

    void setFont(const uint8_t* font);
    void setFontSize(int size);

    int getXOffset() const { return xOffset; }
    int getYOffset() const { return yOffset; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }

private:
    U8G2* u8g2; 
    const DisplayConfig& config; 
    EDGELogger _logger;

    int width = 128;
    int height = 64;

    int xOffset = 0;
    int yOffset = 0;
};