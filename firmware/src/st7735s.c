
#include "st7735s.h"
#include "spi.h"
#include "gpio.h"
#include "delay.h"
#include "font.h"
#include "board_config.h"

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <avr/pgmspace.h>

// Display control pins
static const gpioPin csPin = GPIO_PIN(ST7735S_CS_PORT_LETTER, ST7735S_CS_PIN_NUM);
static const gpioPin dcPin = GPIO_PIN(ST7735S_DC_PORT_LETTER, ST7735S_DC_PIN_NUM);
static const gpioPin rstPin = GPIO_PIN(ST7735S_RST_PORT_LETTER, ST7735S_RST_PIN_NUM);
static const gpioPin blPin = GPIO_PIN(ST7735S_BL_PORT_LETTER, ST7735S_BL_PIN_NUM);

// ST7735S commands
#define CASET   0x2A
#define RASET   0x2B
#define RAMWR   0x2C
#define SLPOUT  0x11
#define DISPON  0x29
#define MADCTL  0x36
#define SWRESET 0x01
#define COLMOD  0x3A
#define RGB565  0x05

// Display offsets
#define X_OFFSET 1
#define Y_OFFSET 2

// Private function declarations
static void writeCommand(uint8_t command);
static void writeData(uint8_t data);
static void writeData16(uint16_t data);
static void setAddressWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
static void hardwareReset(void);

static void drawPixel(int16_t x, int16_t y, uint16_t color);
static void drawCharScaled(int16_t x, int16_t y, char character, uint16_t color, uint8_t scale);

void st7735sInit(void)
{
    // Configure display control pins
    gpioSetMode(&csPin, GPIO_OUTPUT);
    gpioSetMode(&dcPin, GPIO_OUTPUT);
    gpioSetMode(&rstPin, GPIO_OUTPUT);
    gpioSetMode(&blPin, GPIO_OUTPUT);

    gpioWrite(&csPin, true);
    gpioWrite(&dcPin, true);
    gpioWrite(&rstPin, true);
    gpioWrite(&blPin, true);

    spiInit();
    hardwareReset();

    // Software reset
    gpioWrite(&csPin, false);
    writeCommand(SWRESET);
    gpioWrite(&csPin, true);
    delayMs(150);

    /// Frame rate control
    gpioWrite(&csPin, false);
    writeCommand(0xB1);
    writeData(0x01);
    writeData(0x2C);
    writeData(0x2D);
    gpioWrite(&csPin, true);

    gpioWrite(&csPin, false);
    writeCommand(0xB2);
    writeData(0x01);
    writeData(0x2C);
    writeData(0x2D);
    gpioWrite(&csPin, true);

    gpioWrite(&csPin, false);
    writeCommand(0xB3);
    writeData(0x01);
    writeData(0x2C);
    writeData(0x2D);
    writeData(0x01);
    writeData(0x2C);
    writeData(0x2D);
    gpioWrite(&csPin, true);

    // Display inversion control
    gpioWrite(&csPin, false);
    writeCommand(0xB4);
    writeData(0x07);
    gpioWrite(&csPin, true);

    // Power control
    gpioWrite(&csPin, false);
    writeCommand(0xC0);
    writeData(0xA2);
    writeData(0x02);
    writeData(0x84);
    gpioWrite(&csPin, true);

    gpioWrite(&csPin, false);
    writeCommand(0xC1);
    writeData(0xC5);
    gpioWrite(&csPin, true);

    gpioWrite(&csPin, false);
    writeCommand(0xC2);
    writeData(0x0A);
    writeData(0x00);
    gpioWrite(&csPin, true);

    gpioWrite(&csPin, false);
    writeCommand(0xC3);
    writeData(0x8A);
    writeData(0x2A);
    gpioWrite(&csPin, true);

    gpioWrite(&csPin, false);
    writeCommand(0xC4);
    writeData(0x8A);
    writeData(0xEE);
    gpioWrite(&csPin, true);

    gpioWrite(&csPin, false);
    writeCommand(0xC5);
    writeData(0x0E);
    gpioWrite(&csPin, true);

    // Positive gamma correction
    static const uint8_t positiveGamma[] = {
        0x0F, 0x1A, 0x0F, 0x18,
        0x2F, 0x28, 0x20, 0x22,
        0x1F, 0x1B, 0x23, 0x37,
        0x00, 0x07, 0x02, 0x10
    };

    gpioWrite(&csPin, false);
    writeCommand(0xE0);

    for (uint8_t i = 0; i < sizeof(positiveGamma); i++)
    {
        writeData(positiveGamma[i]);
    }

    gpioWrite(&csPin, true);

    // Negative gamma correction
    static const uint8_t negativeGamma[] = {
        0x0F, 0x1B, 0x0F, 0x17,
        0x33, 0x2C, 0x29, 0x2E,
        0x30, 0x30, 0x39, 0x3F,
        0x00, 0x07, 0x03, 0x10
    };

    gpioWrite(&csPin, false);
    writeCommand(0xE1);

    for (uint8_t i = 0; i < sizeof(negativeGamma); i++)
    {
        writeData(negativeGamma[i]);
    }

    gpioWrite(&csPin, true);

    // Enable test command
    gpioWrite(&csPin, false);
    writeCommand(0xF0);
    writeData(0x01);
    gpioWrite(&csPin, true);

    // Disable RAM power save mode
    gpioWrite(&csPin, false);
    writeCommand(0xF6);
    writeData(0x00);
    gpioWrite(&csPin, true);

    // Set RGB565 pixel format
    gpioWrite(&csPin, false);
    writeCommand(COLMOD);
    writeData(RGB565);
    gpioWrite(&csPin, true);

    // Set display orientation
    gpioWrite(&csPin, false);
    writeCommand(MADCTL);
    writeData(0x60);
    gpioWrite(&csPin, true);

    // Exit sleep mode
    gpioWrite(&csPin, false);
    writeCommand(SLPOUT);
    gpioWrite(&csPin, true);
    delayMs(200);

    // Turn display on
    gpioWrite(&csPin, false);
    writeCommand(DISPON);
    gpioWrite(&csPin, true);
    delayMs(200);
}

void st7735sFillScreen(uint16_t color)
{
    gpioWrite(&csPin, false);

    setAddressWindow(0, 0, ST7735S_WIDTH - 1, ST7735S_HEIGHT - 1);
    writeCommand(RAMWR);

    uint16_t amountOfPixels = ST7735S_WIDTH * ST7735S_HEIGHT;

    for (uint16_t i = 0; i < amountOfPixels; i++)
    {
        writeData16(color);
    }

    gpioWrite(&csPin, true);
}

void st7735sFillRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint16_t color)
{
    if (width == 0 || height == 0)
    {
        return;
    }

    uint16_t x1 = (uint16_t)x + width - 1;
    uint16_t y1 = (uint16_t)y + height - 1;

    if (x >= ST7735S_WIDTH || y >= ST7735S_HEIGHT ||
        x1 >= ST7735S_WIDTH || y1 >= ST7735S_HEIGHT)
    {
        return;
    }

    gpioWrite(&csPin, false);

    setAddressWindow(x, y, (uint8_t)x1, (uint8_t)y1);
    writeCommand(RAMWR);

    uint16_t amountOfPixels = (uint16_t)width * height;

    for (uint16_t i = 0; i < amountOfPixels; i++)
    {
        writeData16(color);
    }

    gpioWrite(&csPin, true);
}

void st7735sFillCircle(int16_t x, int16_t y, uint8_t radius, uint16_t color)
{
    int16_t startX = x - radius;
    int16_t endX = x + radius;
    int16_t startY = y - radius;
    int16_t endY = y + radius;

    int32_t radiusSquared = (int32_t)radius * radius;

    for (int16_t pixelX = startX; pixelX <= endX; pixelX++)
    {
        for (int16_t pixelY = startY; pixelY <= endY; pixelY++)
        {
            int32_t deltaX = pixelX - x;
            int32_t deltaY = pixelY - y;
            int32_t distanceSquared = deltaX * deltaX + deltaY * deltaY;

            if (distanceSquared <= radiusSquared)
            {
                drawPixel(pixelX, pixelY, color);
            }
        }
    }
}

void st7735sDrawString(uint8_t x, uint8_t y, const char *text, uint16_t color, uint8_t scale)
{
    if (text == NULL || scale == 0)
    {
        return;
    }

    int16_t cursorX = x;

    while (*text != '\0')
    {
        const fontGlyph *glyph = fontGetGlyph(&FONT_5X7, *text);

        if (glyph != NULL)
        {
            drawCharScaled(cursorX, y, *text, color, scale);
            cursorX += (glyph->width + 1) * scale;
        }

        text++;
    }
}

uint16_t st7735sGetStringWidth(const char *text, uint8_t scale)
{
    if (text == NULL || scale == 0)
    {
        return 0;
    }

    uint16_t width = 0;

    while (*text != '\0')
    {
        const fontGlyph *glyph = fontGetGlyph(&FONT_5X7, *text);

        if (glyph != NULL)
        {
            width += (uint16_t)(glyph->width + 1) * scale;
        }

        text++;
    }

    if (width > 0)
    {
        width -= scale;
    }

    return width;
}

// Helpers
static void drawPixel(int16_t x, int16_t y, uint16_t color)
{
    if (x < 0 || y < 0 || x >= ST7735S_WIDTH || y >= ST7735S_HEIGHT)
    {
        return;
    }

    gpioWrite(&csPin, false);

    setAddressWindow((uint8_t)x, (uint8_t)y, (uint8_t)x, (uint8_t)y);
    writeCommand(RAMWR);
    writeData16(color);

    gpioWrite(&csPin, true);
}

static void drawCharScaled(int16_t x, int16_t y, char character, uint16_t color, uint8_t scale)
{
    if (scale == 0)
    {
        return;
    }

    const fontGlyph *glyph = fontGetGlyph(&FONT_5X7, character);

    if (glyph == NULL)
    {
        return;
    }

    for (uint8_t row = 0; row < glyph->height; row++)
    {
        uint8_t bitmapRow = pgm_read_byte(&glyph->bitmap[row]);

        for (uint8_t col = 0; col < glyph->width; col++)
        {
            uint8_t mask = (uint8_t)(1U << (glyph->width - 1U - col));

            if ((bitmapRow & mask) != 0)
            {
                int16_t pixelX = x + (int16_t)col * scale;
                int16_t pixelY = y + (int16_t)row * scale;

                if (pixelX < 0 || pixelY < 0 ||
                    pixelX + scale > ST7735S_WIDTH ||
                    pixelY + scale > ST7735S_HEIGHT)
                {
                    continue;
                }

                st7735sFillRect((uint8_t)pixelX, (uint8_t)pixelY, scale, scale, color);
            }
        }
    }
}

static void setAddressWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
    // Column address
    writeCommand(CASET);
    writeData16(x0 + X_OFFSET);
    writeData16(x1 + X_OFFSET);

    // Row address
    writeCommand(RASET);
    writeData16(y0 + Y_OFFSET);
    writeData16(y1 + Y_OFFSET);
}

static void writeCommand(uint8_t command)
{
    gpioWrite(&dcPin, false);
    spiTransfer(command);
}

static void writeData(uint8_t data)
{
    gpioWrite(&dcPin, true);
    spiTransfer(data);
}

static void writeData16(uint16_t data)
{
    writeData((uint8_t)(data >> 8));
    writeData((uint8_t)(data & 0xFF));
}

static void hardwareReset(void)
{
    gpioWrite(&rstPin, true);
    delayMs(10);

    gpioWrite(&rstPin, false);
    delayMs(20);

    gpioWrite(&rstPin, true);
    delayMs(120);
}
