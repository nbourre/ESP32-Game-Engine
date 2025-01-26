#include "Renderer.h"

#define I2C_SDA 5
#define I2C_SCL 6

Renderer::Renderer() : u8g2(U8G2_R0, U8X8_PIN_NONE, I2C_SCL, I2C_SDA) {}

void Renderer::init() {
    u8g2.begin();
    u8g2.setContrast(255);
    u8g2.setFont(u8g2_font_ncenB10_tr);  // Set a default font
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