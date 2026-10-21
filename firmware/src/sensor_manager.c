#include "sensor_manager.h"
#include "sensor_data.h"
#include "adc.h"

#ifndef LDR_CHANNEL
    #define LDR_CHANNEL 0
#endif

static sensorPayload currentPayload;

void sensorManagerInit()
{
    adcInit(LDR_CHANNEL);
}

void sensorManagerReadSensors()
{
    currentPayload.temperature = 190;
    currentPayload.brightness = adcRead(LDR_CHANNEL);
}

const sensorPayload* sensorManagerGetData(void)
{
    return &currentPayload;
}