#ifndef DS18B20_H
#define DS18B20_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Initializes the DS18B20 temperature sensor.
 *
 * @return true if the sensor responds on the 1-Wire bus,
 *         otherwise false.
 */
bool ds18b20Init(void);

/**
 * @brief Starts an asynchronous temperature conversion.
 *
 * Returns without waiting for the temperature conversion to complete.
 */
void ds18b20StartConversion(void);

/**
 * @brief Reads the raw temperature value from the DS18B20.
 *
 * @return Raw temperature value in units of 1/16 °C
 *         when using the default 12-bit resolution.
 */
int16_t ds18b20ReadRaw(void);

#endif // DS18B20_H