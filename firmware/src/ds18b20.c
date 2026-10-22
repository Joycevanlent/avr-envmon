#include "ds18b20.h"
#include "onewire.h"
#include "crc8.h"

#include <stddef.h>

sensorStatus ds18b20Init(void)
{
    if (!oneWireReset())
    {
        return SENSOR_ERR_NO_PRESENCE;
    }

    return SENSOR_OK;
}

sensorStatus ds18b20StartConversion(void)
{
    if (!oneWireReset())
    {
        return SENSOR_ERR_NO_PRESENCE;
    }

    oneWireWriteByte(0xCC); // Skip ROM
    oneWireWriteByte(0x44); // Convert T

    return SENSOR_OK;
}

sensorStatus ds18b20Read(int16_t *rawTemperature)
{
    if (rawTemperature == NULL)
    {
        return SENSOR_ERR_INVALID_VALUE;
    }

    if (!oneWireReset())
    {
        return SENSOR_ERR_NO_PRESENCE;
    }

    oneWireWriteByte(0xCC); // Skip ROM
    oneWireWriteByte(0xBE); // Read Scratchpad

    uint8_t scratchpad[9];
    for (uint8_t i = 0; i < 9; i++)
    {
        scratchpad[i] = oneWireReadByte();
    }

    // CHeck first 8 bytes using the last CRC byte
    uint8_t calculatedCrc = crc8Calculate(scratchpad, 8);
    if (calculatedCrc != scratchpad[8])
    {
        return SENSOR_ERR_CRC;
    }

    // Check the start value (85 °C = 0x0550), which means conversion failed
    int16_t raw = (int16_t)((scratchpad[1] << 8) | scratchpad[0]);
    if (raw == 0x0550)
    {
        return SENSOR_ERR_TIMEOUT; // Conversion not done or invalid value
    }

    *rawTemperature = raw;
    return SENSOR_OK;
}