#include "sensor_manager.h"
#include "sensor_data.h"

static sensorPayload currentPayload;

void sensorManagerInit()
{

}

void sensorManagerReadSensors()
{
    currentPayload.temperature = 190;
    currentPayload.brightness = 1;
}

sensorPayload* sensorManagerGetData(void)
{
    return &currentPayload;
}