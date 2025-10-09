#include "Renderer.h"
#include <SafeString.h>
#include <stdarg.h>  // Required for variadic functions
#include <SPI.h>

Renderer::Renderer(const DisplayConfig& config) : config(config) {
    switch (config.type) {
        case SSD1306:
            if (config.useHardwareI2C) {
                u8g2 = new U8G2_SSD1306_128X64_NONAME_F_HW_I2C(config.rotation, U8X8_PIN_NONE, config.clockPin, config.dataPin);
            } else {
                u8g2 = new U8G2_SSD1306_128X64_NONAME_F_SW_I2C(config.rotation, config.clockPin, config.dataPin, config.resetPin);
            }
            break;

        case SH1106:
            u8g2 = new U8G2_SH1106_128X64_NONAME_F_HW_I2C(config.rotation, U8X8_PIN_NONE, config.clockPin, config.dataPin);
            break;
       //adds support for SSD1309 
        case SSD1309:
            if (config.useHardwareI2C) {
                u8g2 = new U8G2_SSD1309_128X64_NONAME2_F_4W_SW_SPI(config.rotation, config.clockPin, config.dataPin, config.csPin, config.dcPin, config.resetPin);
            } else {
                u8g2 = new U8G2_SSD1309_128X64_NONAME2_F_4W_HW_SPI(config.rotation, config.csPin, config.dcPin, config.resetPin);
            }
            break;
        // Add your own display types here
    }

    xOffset = config.xOffset;
    yOffset = config.yOffset;
}


void Renderer::init() {
    u8g2->begin();
    u8g2->setContrast(255);
    u8g2->setFont(u8g2_font_5x7_tf);

}

void Renderer::beginFrame() {
    u8g2->clearBuffer();
}

void Renderer::endFrame() {
    u8g2->sendBuffer();
}

void Renderer::drawText(int x, int y, const char* str) {
    u8g2->drawStr(xOffset + x, yOffset + y, str);
}

void Renderer::drawTextSafe(int x, int y, const char* format, ...) {
    createSafeString(displayText, 32);  // Define SafeString with a 32-char limit

    va_list args;
    va_start(args, format);
    char buffer[32];  // Temporary buffer for formatting
    vsnprintf(buffer, sizeof(buffer), format, args);  // Format text safely
    va_end(args);

    displayText = buffer;  // Assign formatted text to SafeString

    u8g2->drawStr(xOffset + x, yOffset + y, displayText.c_str());
}

void Renderer::drawCircle(int x, int y, int radius) {
    u8g2->drawDisc(xOffset + x, yOffset + y, radius);
}

void Renderer::drawRectangle(int x, int y, int width, int height) {
    u8g2->drawFrame(xOffset + x, yOffset + y, width, height);
}

void Renderer::drawFilledRectangle(int x, int y, int width, int height) {
    u8g2->drawBox(xOffset + x, yOffset + y, width, height);
}

void Renderer::drawLine(int x1, int y1, int x2, int y2) {
    u8g2->drawLine(xOffset + x1, yOffset + y1, xOffset + x2, yOffset + y2);
}

void Renderer::setFont(const uint8_t* font) {
    u8g2->setFont(font);
}

void Renderer::setFontSize(int size) {
    if (size == 1) {
        u8g2->setFont(u8g2_font_5x7_tf);
    } else if (size == 2) {
        u8g2->setFont(u8g2_font_ncenB08_tr);
    } else {
        u8g2->setFont(u8g2_font_ncenB14_tr);
    }
}

//draw an image to the screen in an XBM format using the u8g2 lib (idk what u want me to say lol)
void Renderer::drawXBM(int x, int y, int width, int height, const uint8_t *bitmap) {
    u8g2->drawXBM(xOffset + x, yOffset + y, width, height, bitmap);
}


