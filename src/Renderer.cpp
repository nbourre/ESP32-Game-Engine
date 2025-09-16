#include "Renderer.h"
#include <stdarg.h>  // Required for variadic functions
#include <U8g2lib.h>
#include <SerialForwarder.h>

extern SerialForwarder* forwardedSerial_ptr; // Use pointer type


// Constructor implementation updated
Renderer::Renderer(U8G2* u8g2_ptr, const DisplayConfig& displayConf)
    : u8g2(u8g2_ptr), config(displayConf), width(displayConf.width), height(displayConf.height)
{
    // Ensure u8g2 pointer is not null
    if (!u8g2) {
        forwardedSerial_ptr->println("[RENDERER] Renderer Error: U8G2 pointer is null!");
        // Handle error appropriately, maybe halt execution or set an error state
    }
     // Calculate offsets based on config if they weren't provided or are default
     xOffset = displayConf.xOffset;
     yOffset = displayConf.yOffset;
}


void Renderer::init() {
    if (!u8g2) return; // Don't proceed if u8g2 is null
    //u8g2->setBusClock(400000);
    u8g2->setBusClock(1000000); // Increased bus clock

    u8g2->begin();
    u8g2->setContrast(255); // Max contrast
    u8g2->setFont(u8g2_font_5x7_tf); // Default font
    forwardedSerial_ptr->println("[RENDERER] Renderer initialized.");
}

void Renderer::beginFrame() {
     if (!u8g2) return;
    u8g2->clearBuffer();
}

void Renderer::endFrame() {
     if (!u8g2) return;
    u8g2->sendBuffer();
}

void Renderer::drawText(int x, int y, const char* str) {
     if (!u8g2) return;
    u8g2->drawStr(xOffset + x, yOffset + y, str);
}

void Renderer::drawTextSafe(int x, int y, const char* format, ...) {
     if (!u8g2) return;
    
    char buffer[32];  // Temporary buffer for formatting
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);  // Format text safely
    va_end(args);

    // The createSafeString() and displayText object were redundant and have been removed.
    // We now use the standard C buffer directly, which is already handled safely by vsnprintf.
    u8g2->drawStr(xOffset + x, yOffset + y, buffer);
}

void Renderer::drawCircle(int x, int y, int radius) {
     if (!u8g2) return;
    // Use drawCircle for outline, drawDisc for filled
    u8g2->drawCircle(xOffset + x, yOffset + y, radius);
}

void Renderer::drawFilledCircle(int x, int y, int radius) {
    if (!u8g2) return;
   // Use drawCircle for outline, drawDisc for filled
   u8g2->drawDisc(xOffset + x, yOffset + y, radius);
}

void Renderer::drawRectangle(int x, int y, int width, int height) {
     if (!u8g2) return;
    u8g2->drawFrame(xOffset + x, yOffset + y, width, height);
}

void Renderer::drawFilledRectangle(int x, int y, int width, int height) {
     if (!u8g2) return;
    u8g2->drawBox(xOffset + x, yOffset + y, width, height);
}

void Renderer::drawLine(int x1, int y1, int x2, int y2) {
     if (!u8g2) return;
    u8g2->drawLine(xOffset + x1, yOffset + y1, xOffset + x2, yOffset + y2);
}

void Renderer::setFont(const uint8_t* font) {
     if (!u8g2) return;
    u8g2->setFont(font);
}

// Keep setFontSize or remove if you prefer only setFont
void Renderer::setFontSize(int size) {
     if (!u8g2) return;
    if (size == 1) {
        u8g2->setFont(u8g2_font_5x7_tf);
    } else if (size == 2) {
        u8g2->setFont(u8g2_font_ncenB08_tr);
    } else {
        // Consider a larger default or specific font for size > 2
        u8g2->setFont(u8g2_font_ncenB14_tr);
    }
}

void Renderer::setContrast(uint8_t level) {
     if (!u8g2) return;
     u8g2->setContrast(level);
}