#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include "sensor_data.h"

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Initializes the sensor manager and connected sensors.
 */
void sensorManagerInit(void);

/**
 * @brief Starts a new asynchronous measurement cycle.
 *
 * Initiates temperature conversion and records the start time
 * of the measurement cycle.
 */
void sensorManagerStartMeasurement(void);

/**
 * @brief Checks whether the current measurement cycle is complete.
 *
 * @param nowMillis Current system time in milliseconds.
 * @return true if fresh sensor data is available, otherwise false.
 */
bool sensorManagerIsDataReady(uint32_t nowMillis);

/**
 * @brief Returns the latest sensor measurements.
 *
 * @return Pointer to the current sensor payload.
 */
const sensorPayload *sensorManagerGetData(void);

#endif // SENSOR_MANAGER_H