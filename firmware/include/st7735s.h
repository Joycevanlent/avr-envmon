/**
 * @file st7735s.h
 * @brief Driver interface for the ST7735S TFT display controller.
 *
 * Provides display initialization and basic drawing operations.
 * Hardware-specific SPI and GPIO operations are provided by
 * the selected platform drivers.
 */

#ifndef ST7735S_H
#define ST7735S_H

#include <stdint.h>
#include <stdbool.h>

/* Display dimensions */
#define ST7735S_WIDTH  160
#define ST7735S_HEIGHT 128

/* RGB565 color definitions */
#define ST7735S_COLOR_BLACK 0x0000
#define ST7735S_COLOR_WHITE 0xFFFF
#define ST7735S_COLOR_RED   0xF800
#define ST7735S_COLOR_GREEN 0x07E0
#define ST7735S_COLOR_BLUE  0x001F
#define ST7735S_COLOR_ORANGE 0xFA00

/**
 * @brief Initializes the ST7735S display.
 *
 * Performs hardware reset and sends the controller initialization
 * sequence. Requires SPI and GPIO drivers to be initialized.
 */
void st7735sInit(void);

/**
 * @brief Sets the display address window.
 *
 * @param x0 Starting horizontal coordinate.
 * @param y0 Starting vertical coordinate.
 * @param x1 Ending horizontal coordinate.
 * @param y1 Ending vertical coordinate.
 */
bool st7735sSetAddressWindow(uint8_t x0, uint8_t y0,
                                uint8_t x1, uint8_t y1);

/**
 * @brief Fills the entire display with a single RGB565 color.
 *
 * @param color RGB565 color value.
 */
void st7735sFillScreen(uint16_t color);

#endif // ST7735S_H
