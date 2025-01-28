#include "Renderer.h"
#include <SafeString.h>
#include <stdarg.h>  // Required for variadic functions

void Renderer::init() {
    u8g2.begin();
    u8g2.setContrast(255);
    u8g2.setFont(u8g2_font_5x7_tf);  // Set a default font
}

void Renderer::beginFrame() {
    u8g2.clearBuffer();
}

void Renderer::endFrame() {
    u8g2.sendBuffer();
}

void Renderer::drawText(int x, int y, const char* str) {
    u8g2.drawStr(xOffset + x, yOffset + y, str);
}

void Renderer::drawTextSafe(int x, int y, const char* format, ...) {
    createSafeString(displayText, 32);  // Define SafeString with a 32-char limit

    va_list args;
    va_start(args, format);
    char buffer[32];  // Temporary buffer for formatting
    vsnprintf(buffer, sizeof(buffer), format, args);  // Format text safely
    va_end(args);

    displayText = buffer;  // Assign formatted text to SafeString

    u8g2.drawStr(xOffset + x, yOffset + y, displayText.c_str());
}

void Renderer::drawCircle(int x, int y, int radius) {
    u8g2.drawDisc(xOffset + x, yOffset + y, radius);
}

void Renderer::drawRectangle(int x, int y, int width, int height) {
    u8g2.drawFrame(xOffset + x, yOffset + y, width, height);
}

void Renderer::drawFilledRectangle(int x, int y, int width, int height) {
    u8g2.drawBox(xOffset + x, yOffset + y, width, height);
}

void Renderer::drawLine(int x1, int y1, int x2, int y2) {
    u8g2.drawLine(xOffset + x1, yOffset + y1, xOffset + x2, yOffset + y2);
}

void Renderer::setFont(const uint8_t* font) {
    u8g2.setFont(font);
}
