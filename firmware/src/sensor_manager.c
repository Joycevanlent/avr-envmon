#include "sensor_manager.h"
#include "sensor_data.h"

static sensorPayload currentPayload;

void sensorManager_init()
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