#include "sensor_manager.h"
#include "sensor_data.h"
#include "adc.h"
#include "ds18b20.h"
#include "timer.h"

#include "serial.h"
#include <string.h>

#include <stdbool.h>

#ifndef LDR_CHANNEL
    #define LDR_CHANNEL 0
#endif

static sensorPayload currentPayload;

static uint32_t measurementStartedMillis = 0;
static bool conversionInProgress = false;
static uint16_t measurementTime = 800;

void sensorManagerInit(void)
{
    adcInit(LDR_CHANNEL);

    bool sensorFound = ds18b20Init();

    const char *message = sensorFound
        ? "DS18B20: FOUND\r\n"
        : "DS18B20: NOT FOUND\r\n";

    serialSend(
        (const uint8_t *)message,
        (uint8_t)strlen(message)
    );
}

void sensorManagerStartMeasurement(void)
{
    ds18b20StartConversion();
    measurementStartedMillis = timerMillis();
    conversionInProgress = true;
}

bool sensorManagerIsDataReady(uint32_t nowMillis)
{
    if(!conversionInProgress)
    {
        return false; // check if a measurement is in progress
    }

    if(nowMillis - measurementStartedMillis >= measurementTime)
    {
        currentPayload.temperature = ds18b20ReadRaw();
        currentPayload.brightness = adcRead(LDR_CHANNEL);

        conversionInProgress = false;

        return true;
    }

    return false;
}

const sensorPayload *sensorManagerGetData(void)
{
    return &currentPayload;
}
