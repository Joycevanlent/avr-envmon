/**
 * @file sensor_manager.h
 */

#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include "sensor_data.h"

/**
 * @brief Initializes the sensor manager and all underlying sensor peripherals (e.g., I2C, ADC).
 */
void sensorManagerInit(void);

/**
 * @brief Triggers a new measurement cycle for all active sensors.
 */
void sensorManagerReadSensors(void);

/**
 * @brief Retrieves a pointer to the most recently measured sensor data.
 * 
 * 
 * @return sensorPayload* Pointer to the structure containing the latest measurements.
 */
const sensorPayload* sensorManagerGetData(void);

#endif // SENSOR_MANAGER_H
