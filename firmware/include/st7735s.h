/**
 * @file st7735s.h
 * @brief Driver interface for the ST7735S TFT display controller.
 *
 * Provides display initialization and drawing operations,
 * including rectangle, circle and text rendering.
 *
 * Hardware-specific SPI and GPIO operations are provided
 * by the platform drivers.
 */

#ifndef ST7735S_H
#define ST7735S_H

#include <stdint.h>

/* Display dimensions */
#define ST7735S_WIDTH  160
#define ST7735S_HEIGHT 128

/* RGB565 color definitions */
#define ST7735S_COLOR_BLACK  0x0000
#define ST7735S_COLOR_WHITE  0xFFFF
#define ST7735S_COLOR_RED    0xF800
#define ST7735S_COLOR_GREEN  0x07E0
#define ST7735S_COLOR_BLUE   0x001F
#define ST7735S_COLOR_ORANGE 0xFA00
#define ST7735S_COLOR_YELLOW 0xFFE0

/**
 * @brief Initializes the ST7735S display.
 *
 * Performs hardware reset and sends the controller
 * initialization sequence.
 */
void st7735sInit(void);

/**
 * @brief Fills the entire display with a single RGB565 color.
 *
 * @param color RGB565 color value.
 */
void st7735sFillScreen(uint16_t color);

/**
 * @brief Draws a filled rectangle.
 *
 * The rectangle is only drawn if it fits entirely within
 * the display boundaries.
 *
 * @param x Top-left horizontal coordinate.
 * @param y Top-left vertical coordinate.
 * @param width Rectangle width in pixels.
 * @param height Rectangle height in pixels.
 * @param color RGB565 color value.
 */
void st7735sFillRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint16_t color);

/**
 * @brief Draws a filled circle.
 *
 * Coordinates outside the display boundaries are clipped.
 *
 * @param x Horizontal center coordinate.
 * @param y Vertical center coordinate.
 * @param radius Circle radius in pixels.
 * @param color RGB565 color value.
 */
void st7735sFillCircle(int16_t x, int16_t y, uint8_t radius, uint16_t color);

/**
 * @brief Draws a null-terminated string using the default font.
 *
 * Characters are drawn from left to right with spacing
 * proportional to the selected scale.
 *
 * @param x Horizontal starting coordinate.
 * @param y Vertical starting coordinate.
 * @param text Null-terminated string.
 * @param color RGB565 color value.
 * @param scale Character scaling factor. Must be greater than zero.
 */
void st7735sDrawString(uint8_t x, uint8_t y, const char *text, uint16_t color, uint8_t scale);

/**
 * @brief Calculates the width of a scaled string.
 *
 * @param text Null-terminated string.
 * @param scale Character scaling factor. Must be greater than zero.
 * @return String width in pixels, or zero for invalid input.
 */
uint16_t st7735sGetStringWidth(const char *text, uint8_t scale);

#endif // ST7735S_H