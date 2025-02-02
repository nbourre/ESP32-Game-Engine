#pragma once

#include <U8g2lib.h>

/**
 * @brief Supported display types for the EDGE Renderer.
 */
enum DisplayType {
    SSD1306,  ///< SSD1306 OLED Display (128x64)
    SH1106    ///< SH1106 OLED Display (128x64)
};

/**
 * @brief Configuration settings for initializing U8g2-compatible displays.
 * 
 * Use this structure to define the hardware pins, display dimensions, rotation,
 * and communication type (I2C or SPI).
 */
struct DisplayConfig {
    DisplayType type;               ///< Type of display (e.g., SSD1306, SH1106)
    uint8_t clockPin;               ///< I2C/SPI clock pin (SCL/SCK)
    uint8_t dataPin;                ///< I2C data pin (SDA) or SPI MOSI pin
    uint8_t resetPin;               ///< Reset pin (optional), set to U8X8_PIN_NONE if unused
    const u8g2_cb_t* rotation;      ///< Display rotation (e.g., U8G2_R0, U8G2_R2)
    uint16_t width;                 ///< Display width in pixels
    uint16_t height;                ///< Display height in pixels
    bool useHardwareI2C;            ///< Set to true for hardware I2C, false for software I2C
    int xOffset = 0;                ///< X offset for display rendering
    int yOffset = 0;                ///< Y offset for display rendering

    /**
     * @brief Constructor to initialize the display configuration.
     * 
     * @param displayType Type of the display (SSD1306 or SH1106).
     * @param clk Clock pin (SCL for I2C or SCK for SPI).
     * @param data Data pin (SDA for I2C or MOSI for SPI).
     * @param rst Reset pin (optional, use U8X8_PIN_NONE if not used).
     * @param rot Display rotation (e.g., U8G2_R0, U8G2_R2).
     * @param w Display width in pixels.
     * @param h Display height in pixels.
     * @param hwI2C True to use hardware I2C, false for software I2C.
     */
    DisplayConfig(DisplayType displayType, uint8_t clk, uint8_t data, uint8_t rst,
                  const u8g2_cb_t* rot, uint16_t w, uint16_t h, bool hwI2C = true)
        : type(displayType), clockPin(clk), dataPin(data), resetPin(rst),
          rotation(rot), width(w), height(h), useHardwareI2C(hwI2C) {
            
            const int DEFAULT_DISPLAY_WIDTH = 128;
            const int DEFAULT_DISPLAY_HEIGHT = 64;

            xOffset = (DEFAULT_DISPLAY_WIDTH - width) / 2;
            yOffset = (DEFAULT_DISPLAY_HEIGHT - height) / 2;            

          }
};
