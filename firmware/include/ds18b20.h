/**
 * @file ds18b20.h
 * @brief Provides an interface for the DS18B20 temperature sensor.
 *
 * Provides functions to initialize the sensor, start an asynchronous
 * temperature conversion, and read the resulting temperature data
 * over the 1-Wire bus.
 */

#ifndef DS18B20_H
#define DS18B20_H

#include "sensor_status.h"

#include <stdint.h>

/**
 * @brief Initializes the DS18B20 temperature sensor.
 *
 * Checks whether the sensor responds on the 1-Wire bus.
 *
 * @return SENSOR_OK if the sensor responds successfully,
 *         or SENSOR_ERR_NO_PRESENCE if no sensor is detected.
 */
sensorStatus ds18b20Init(void);

/**
 * @brief Starts an asynchronous temperature conversion.
 *
 * Checks for sensor presence and sends the Skip ROM and Convert T
 * commands. Returns without waiting for the conversion to complete.
 *
 * @return SENSOR_OK if the conversion command is sent successfully,
 *         or SENSOR_ERR_NO_PRESENCE if no sensor is detected.
 */
sensorStatus ds18b20StartConversion(void);

/**
 * @brief Reads the raw temperature value from the DS18B20 sensor.
 *
 * Reads the sensor scratchpad, validates its CRC, and stores the raw
 * temperature value in the provided output parameter.
 *
 * @param[out] rawTemperature Pointer to the variable that receives
 *                            the raw temperature value. Must not be NULL.
 *
 * @return SENSOR_OK if the temperature is read successfully.
 * @return SENSOR_ERR_INVALID_VALUE if rawTemperature is NULL.
 * @return SENSOR_ERR_NO_PRESENCE if no sensor is detected.
 * @return SENSOR_ERR_CRC if the scratchpad CRC check fails.
 * @return SENSOR_ERR_TIMEOUT if the raw value matches the sensor's
 *         85 °C power-on default value.
 */
sensorStatus ds18b20Read(int16_t *rawTemperature);

#endif // DS18B20_H