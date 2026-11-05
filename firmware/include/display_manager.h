/**
 * @file display_manager.h
 * @brief Display management interface for the environment monitor.
 *
 * Provides functions to initialize the display and update
 * temperature and ambient light visualizations.
 */

#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <stdint.h>

/**
 * @brief Initializes the display manager.
 *
 * Initializes the ST7735S display and clears the screen.
 */
void displayManagerInit(void);

/**
 * @brief Displays the current temperature.
 *
 * The display is only updated when the temperature changes.
 *
 * @param temperatureTenths Temperature in tenths of degrees Celsius.
 *                           For example, 235 represents 23.5 degrees Celsius.
 */
void displayManagerShowTemperature(int16_t temperatureTenths);

/**
 * @brief Displays the current ambient light level.
 *
 * Displays a moon, half-moon or sun icon depending on the
 * ambient light level. The icon is only redrawn when its
 * category changes.
 *
 * @param lightLevel Ambient light ADC reading (0-1023).
 */
void displayManagerShowLightLevel(uint16_t lightLevel);

#endif