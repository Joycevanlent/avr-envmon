/**
 * @file sensor_data.h
 * @brief Defines the data structure for environmental sensor measurements.
 *
 * Contains the measurements collected by the environmental monitor,
 * including temperature and light sensor readings.
 */

#ifndef SENSOR_DATA_H
#define SENSOR_DATA_H

#include <stdint.h>

/**
 * @brief Contains the latest environmental sensor measurements.
 *
 * Temperature is represented in tenths of a degree Celsius.
 * For example, a value of 235 represents 23.5 °C, and -55 represents
 * -5.5 °C.
 *
 * Brightness contains the raw ADC reading from the light sensor,
 * ranging from 0 to 1023 for a 10-bit ADC.
 */
typedef struct
{
    int16_t temperature;  /**< Temperature in tenths of a degree Celsius. */
    uint16_t brightness;  /**< Raw light sensor ADC reading (0–1023). */
} sensorPayload;

#endif // SENSOR_DATA_H
