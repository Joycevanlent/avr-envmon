/**
 * @file sensor_manager.h
 * @brief Manages sensor initialization and asynchronous measurements.
 *
 * Provides a hardware-independent interface for initializing connected
 * sensors, starting measurement cycles, checking measurement completion,
 * and retrieving the latest sensor data.
 */

#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include "sensor_data.h"
#include "sensor_status.h"

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Initializes the sensor manager and connected sensors.
 *
 * Prepares the sensors for measurement and initializes the internal
 * state of the sensor manager.
 *
 * @return SENSOR_OK if initialization succeeds, or an error status
 *         if initialization fails.
 */
sensorStatus sensorManagerInit(void);

/**
 * @brief Starts a new asynchronous measurement cycle.
 *
 * Initiates temperature conversion and records the start time of the
 * measurement cycle. The function returns without waiting for the
 * measurement to complete.
 *
 * @return SENSOR_OK if the measurement starts successfully, or an
 *         error status if the measurement cannot be started.
 */
sensorStatus sensorManagerStartMeasurement(void);

/**
 * @brief Checks whether the current measurement cycle is complete.
 *
 * Updates the measurement state using the supplied system time.
 * This function can be called repeatedly while waiting for fresh data.
 *
 * @param[in] nowMillis Current system time in milliseconds.
 * @param[out] dataReady Set to true when fresh sensor data is available,
 *                       or false when the measurement is still pending.
 *
 * @return SENSOR_OK if the check succeeds, or an error status if
 *         checking the measurement fails.
 */
sensorStatus sensorManagerIsDataReady(uint32_t nowMillis, bool *dataReady);

/**
 * @brief Retrieves the latest sensor measurements.
 *
 * Returns a pointer to the sensor payload maintained by the sensor
 * manager.
 *
 * @return Pointer to the latest sensor payload, or NULL if no payload
 *         is available.
 */
const sensorPayload *sensorManagerGetData(void);

#endif // SENSOR_MANAGER_H