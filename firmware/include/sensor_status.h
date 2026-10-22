/**
 * @file sensor_status.h
 * @brief Defines status codes for sensor operations.
 *
 * Provides a common set of status codes used by the sensor drivers
 * and sensor manager to report successful operations and errors.
 */

#ifndef SENSOR_STATUS_H
#define SENSOR_STATUS_H

/**
 * @brief Represents the result of a sensor operation.
 *
 * SENSOR_OK indicates success. All other values represent an error
 * encountered during sensor communication, measurement, or initialization.
 */
typedef enum
{
    SENSOR_OK = 0,             /**< Operation completed successfully. */
    SENSOR_ERR_NO_PRESENCE,    /**< No 1-Wire device presence pulse detected. */
    SENSOR_ERR_CRC,            /**< Received data failed CRC validation. */
    SENSOR_ERR_TIMEOUT,        /**< Operation timed out or data is not ready. */
    SENSOR_ERR_INVALID_VALUE,  /**< An invalid argument or value was provided. */
    SENSOR_ERR_INIT_LDR,       /**< Light-dependent resistor initialization failed. */
    SENSOR_ERR_INIT_DS18B20    /**< DS18B20 temperature sensor initialization failed. */
} sensorStatus;

#endif // SENSOR_STATUS_H
