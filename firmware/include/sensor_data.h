#ifndef SENSOR_DATA_H
#define SENSOR_DATA_H

#include <stdint.h>

typedef struct {
    int16_t temperature;
    uint16_t brightness;
} sensorPayload;

#endif // SENSOR_DATA_H