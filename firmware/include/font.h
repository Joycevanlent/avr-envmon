/**
 * @file font.h
 * @brief Bitmap font definitions and glyph lookup interface.
 *
 * Provides data structures for bitmap fonts and functions
 * to retrieve individual character glyphs.
 */

#ifndef FONT_H
#define FONT_H

#include <stdint.h>

/**
 * @brief Represents a single bitmap character.
 */
typedef struct
{
    char character;
    uint8_t width;
    uint8_t height;
    const uint8_t *bitmap;
} fontGlyph;

/**
 * @brief Represents a bitmap font.
 */
typedef struct
{
    const fontGlyph *glyphs;
    uint8_t glyphCount;
} font;

/**
 * @brief Default 5x7 font with digits and temperature symbols.
 */
extern const font FONT_5X7;

/**
 * @brief Retrieves a glyph for the requested character.
 *
 * @param selectedFont Font to search in.
 * @param character Character to retrieve.
 * @return Pointer to the glyph, or NULL if not found.
 */
const fontGlyph *fontGetGlyph(const font *selectedFont, char character);

#endif