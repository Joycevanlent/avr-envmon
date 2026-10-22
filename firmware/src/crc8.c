#include "crc8.h"

#include <stdbool.h>

uint8_t crc8Calculate(const uint8_t *data, size_t length)
{
    uint8_t crc = 0x00;

    if (data == NULL && length > 0)
    {
        return crc;
    }

    for (size_t i = 0; i < length; i++)
    {
        uint8_t byte = data[i];
        crc = crc ^ byte;

        for (uint8_t j = 0; j < 8; j++)
        {
            bool lowestBit = crc & 1;
            crc = crc >> 1;

            if (lowestBit == true)
            {
                crc = crc ^ 0x8C;
            }
        }
    }

    return crc;
}
