
#include "display_manager.h"
#include "st7735s.h"

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define TEMPERATURE_SCALE 2
#define TEMPERATURE_AREA_Y 54
#define TEMPERATURE_AREA_HEIGHT 20

#define LIGHT_ICON_CENTER_X 140
#define LIGHT_ICON_CENTER_Y 20
#define LIGHT_ICON_AREA_X 120
#define LIGHT_ICON_AREA_Y 0
#define LIGHT_ICON_AREA_WIDTH 40
#define LIGHT_ICON_AREA_HEIGHT 40

typedef enum
{
    LIGHT_ICON_MOON,
    LIGHT_ICON_HALF_MOON,
    LIGHT_ICON_SUN
} lightIcon;

static int16_t previousTemperature = INT16_MIN;
static lightIcon previousLightIcon;
static bool lightIconInitialized = false;

static void drawSun(void);
static void drawMoon(void);
static void drawHalfMoon(void);
static lightIcon getLightIcon(uint16_t lightLevel);

void displayManagerInit(void)
{
    st7735sInit();

    st7735sFillScreen(ST7735S_COLOR_BLACK);
}

void displayManagerShowTemperature(int16_t temperatureTenths)
{
    if (temperatureTenths == previousTemperature)
    {
        return;
    }

    char temperatureText[10];

    // Determine sign and absolute temperature.
    const char *sign = temperatureTenths < 0 ? "-" : "";
    int32_t magnitude = temperatureTenths;

    if (magnitude < 0)
    {
        magnitude = -magnitude;
    }

    // Split temperature into integer and decimal parts.
    uint16_t integerPart = (uint16_t)(magnitude / 10);
    uint8_t decimalPart = (uint8_t)(magnitude % 10);

    // Format temperature, degree symbol and Celsius.
    int textLength = snprintf(temperatureText, sizeof(temperatureText), "%s%u.%u%cC", sign, (unsigned int)integerPart, (unsigned int)decimalPart, (char)0x7F);

    if (textLength < 0 || (size_t)textLength >= sizeof(temperatureText))
    {
        return;
    }

    // Calculate actual text width.
    uint16_t textWidth = st7735sGetStringWidth(temperatureText, TEMPERATURE_SCALE);

    if (textWidth > ST7735S_WIDTH)
    {
        return;
    }

    // Center temperature text horizontally and vertically.
    uint8_t x = (ST7735S_WIDTH - textWidth) / 2;
    uint8_t textHeight = 7 * TEMPERATURE_SCALE;
    uint8_t y = (ST7735S_HEIGHT - textHeight) / 2;

    // Clear previous temperature.
    st7735sFillRect(0, TEMPERATURE_AREA_Y, ST7735S_WIDTH, TEMPERATURE_AREA_HEIGHT, ST7735S_COLOR_BLACK);

    // Draw new temperature.
    st7735sDrawString(x, y, temperatureText, ST7735S_COLOR_WHITE, TEMPERATURE_SCALE);

    previousTemperature = temperatureTenths;
}

void displayManagerShowLightLevel(uint16_t lightLevel)
{
    lightIcon currentIcon = getLightIcon(lightLevel);

    // Only redraw when the icon category changes.
    if (lightIconInitialized && currentIcon == previousLightIcon)
    {
        return;
    }

    // Clear previous icon.
    st7735sFillRect(LIGHT_ICON_AREA_X, LIGHT_ICON_AREA_Y, LIGHT_ICON_AREA_WIDTH, LIGHT_ICON_AREA_HEIGHT, ST7735S_COLOR_BLACK);

    switch (currentIcon)
    {
    case LIGHT_ICON_MOON:
        drawMoon();
        break;

    case LIGHT_ICON_HALF_MOON:
        drawHalfMoon();
        break;

    case LIGHT_ICON_SUN:
        drawSun();
        break;
    }

    previousLightIcon = currentIcon;
    lightIconInitialized = true;
}

static lightIcon getLightIcon(uint16_t lightLevel)
{
    if (lightLevel <= 299)
    {
        return LIGHT_ICON_MOON;
    }
    else if (lightLevel <= 649)
    {
        return LIGHT_ICON_HALF_MOON;
    }
    else
    {
        return LIGHT_ICON_SUN;
    }
}

static void drawSun(void)
{
    // Vertical rays.
    st7735sFillRect(139, 5, 3, 6, ST7735S_COLOR_ORANGE);
    st7735sFillRect(139, 29, 3, 6, ST7735S_COLOR_ORANGE);

    // Horizontal rays.
    st7735sFillRect(125, 19, 6, 3, ST7735S_COLOR_ORANGE);
    st7735sFillRect(149, 19, 6, 3, ST7735S_COLOR_ORANGE);

    // Diagonal rays.
    st7735sFillRect(130, 9, 4, 4, ST7735S_COLOR_ORANGE);
    st7735sFillRect(146, 9, 4, 4, ST7735S_COLOR_ORANGE);
    st7735sFillRect(130, 27, 4, 4, ST7735S_COLOR_ORANGE);
    st7735sFillRect(146, 27, 4, 4, ST7735S_COLOR_ORANGE);

    // Sun body.
    st7735sFillCircle(LIGHT_ICON_CENTER_X, LIGHT_ICON_CENTER_Y, 8, ST7735S_COLOR_YELLOW);
}

static void drawMoon(void)
{
    st7735sFillCircle(LIGHT_ICON_CENTER_X, LIGHT_ICON_CENTER_Y, 9, ST7735S_COLOR_WHITE);
    st7735sFillCircle(LIGHT_ICON_CENTER_X + 5, LIGHT_ICON_CENTER_Y - 4, 9, ST7735S_COLOR_BLACK);
}

static void drawHalfMoon(void)
{
    st7735sFillCircle(LIGHT_ICON_CENTER_X, LIGHT_ICON_CENTER_Y, 9, ST7735S_COLOR_WHITE);

    // Delete left half.
    st7735sFillRect(130, 11, 10, 18, ST7735S_COLOR_BLACK);
}