#include "sensor_manager.h"
#include "sensor_data.h"
#include "sensor_status.h"
#include "board_config.h"
#include "adc.h"
#include "ds18b20.h"
#include "timer.h"

#include <stdbool.h>
#include <stddef.h>

#ifndef LDR_CHANNEL
#define LDR_CHANNEL 0
#endif

static sensorPayload currentPayload;
static uint32_t measurementStartedMillis = 0;
static bool conversionInProgress = false;
static uint16_t measurementTime = 800;

sensorStatus sensorManagerInit(void)
{
    adcInit(LDR_CHANNEL);

    sensorStatus status = ds18b20Init();

    if (status != SENSOR_OK)
    {
        return SENSOR_ERR_INIT_DS18B20;
    }

    return SENSOR_OK;
}

sensorStatus sensorManagerStartMeasurement(void)
{
    sensorStatus status = ds18b20StartConversion();

    if (status != SENSOR_OK)
    {
        conversionInProgress = false;

        return status;
    }

    measurementStartedMillis = timerMillis();
    conversionInProgress = true;

    return SENSOR_OK;
}

sensorStatus sensorManagerIsDataReady(uint32_t nowMillis, bool *dataReady)
{
    if (dataReady == NULL)
    {
        return SENSOR_ERR_INVALID_VALUE;
    }

    *dataReady = false;

    if (!conversionInProgress)
    {
        return SENSOR_OK;
    }

    if ((nowMillis - measurementStartedMillis) >= measurementTime)
    {
        int16_t rawTemperature = 0;

        sensorStatus status = ds18b20Read(&rawTemperature);
        if (status != SENSOR_OK)
        {
            conversionInProgress = false;
            return status;
        }

        int16_t temperature = (rawTemperature * 10) / 16;

        currentPayload.temperature = temperature;
        currentPayload.brightness = adcRead(LDR_CHANNEL);

        conversionInProgress = false;
        *dataReady = true;

        return SENSOR_OK;
    }

    return SENSOR_OK;
}

const sensorPayload *sensorManagerGetData(void)
{
    return &currentPayload;
}
