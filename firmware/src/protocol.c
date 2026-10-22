#include "protocol.h"

#include <stdio.h>

bool protocolFormatSensorPayload(
    char *buffer,
    size_t bufferSize,
    const sensorPayload *payload
)
{
    if (buffer == NULL || bufferSize == 0 || payload == NULL) {
        return false;
    }

    int16_t temperature = payload->temperature;
    int16_t whole = temperature / 10;
    int16_t fraction = temperature % 10;

    // Keep the fractional digit positive, even for negative temperatures.
    if (fraction < 0) {
        fraction = -fraction;
    }

    int length = snprintf(
        buffer,
        bufferSize,
        "V1,DATA,TEMP=%s%d.%d,LIGHT=%u\r\n",
        temperature < 0 ? "-" : "",
        whole < 0 ? -whole : whole,
        fraction,
        payload->brightness
    );

    return length >= 0 && (size_t)length < bufferSize;
}

bool protocolFormatSensorError(
    char *buffer,
    size_t bufferSize
)
{
    if (buffer == NULL || bufferSize == 0) {
        return false;
    }

    int length = snprintf(
        buffer,
        bufferSize,
        "V1,ERR,SENSOR\r\n"
    );

    return length >= 0 && (size_t)length < bufferSize;
}