#include "font.h"

#include <avr/pgmspace.h>

/* Digits 0-9 */

static const uint8_t digit0[] PROGMEM = {
    0b01110,
    0b10001,
    0b10011,
    0b10101,
    0b11001,
    0b10001,
    0b01110
};

static const uint8_t digit1[] PROGMEM = {
    0b00100,
    0b01100,
    0b00100,
    0b00100,
    0b00100,
    0b00100,
    0b01110
};

static const uint8_t digit2[] PROGMEM = {
    0b01110,
    0b10001,
    0b00001,
    0b00010,
    0b00100,
    0b01000,
    0b11111
};

static const uint8_t digit3[] PROGMEM = {
    0b11110,
    0b00001,
    0b00001,
    0b01110,
    0b00001,
    0b00001,
    0b11110
};

static const uint8_t digit4[] PROGMEM = {
    0b00010,
    0b00110,
    0b01010,
    0b10010,
    0b11111,
    0b00010,
    0b00010
};

static const uint8_t digit5[] PROGMEM = {
    0b11111,
    0b10000,
    0b10000,
    0b11110,
    0b00001,
    0b00001,
    0b11110
};

static const uint8_t digit6[] PROGMEM = {
    0b01110,
    0b10000,
    0b10000,
    0b11110,
    0b10001,
    0b10001,
    0b01110
};

static const uint8_t digit7[] PROGMEM = {
    0b11111,
    0b00001,
    0b00010,
    0b00100,
    0b01000,
    0b01000,
    0b01000
};

static const uint8_t digit8[] PROGMEM = {
    0b01110,
    0b10001,
    0b10001,
    0b01110,
    0b10001,
    0b10001,
    0b01110
};

static const uint8_t digit9[] PROGMEM = {
    0b01110,
    0b10001,
    0b10001,
    0b01111,
    0b00001,
    0b00001,
    0b01110
};

/* Additional characters */

static const uint8_t minus[] PROGMEM = {
    0b00000,
    0b00000,
    0b00000,
    0b11111,
    0b00000,
    0b00000,
    0b00000
};

static const uint8_t dot[] PROGMEM = {
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00100,
    0b00100
};

static const uint8_t letterC[] PROGMEM = {
    0b01110,
    0b10001,
    0b10000,
    0b10000,
    0b10000,
    0b10001,
    0b01110
};

static const uint8_t degree[] PROGMEM = {
    0b010,
    0b101,
    0b010
};

/**
 * @brief Glyph lookup table.
 *
 * Contains all supported characters and their bitmap data.
 */
static const fontGlyph glyphs[] = {
    {'0', 5, 7, digit0},
    {'1', 5, 7, digit1},
    {'2', 5, 7, digit2},
    {'3', 5, 7, digit3},
    {'4', 5, 7, digit4},
    {'5', 5, 7, digit5},
    {'6', 5, 7, digit6},
    {'7', 5, 7, digit7},
    {'8', 5, 7, digit8},
    {'9', 5, 7, digit9},
    {'-', 5, 7, minus},
    {'.', 5, 7, dot},
    {'C', 5, 7, letterC},
    {'°', 3, 3, degree}
};

/**
 * @brief Default font definition.
 */
const font FONT_5X7 = {
    .glyphs = glyphs,
    .glyphCount = sizeof(glyphs) / sizeof(glyphs[0])
};

const fontGlyph *fontGetGlyph(const font *selectedFont, char character)
{
    if (selectedFont == 0)
    {
        return 0;
    }

    for (uint8_t i = 0; i < selectedFont->glyphCount; i++)
    {
        if (selectedFont->glyphs[i].character == character)
        {
            return &selectedFont->glyphs[i];
        }
    }

    return 0;
}
