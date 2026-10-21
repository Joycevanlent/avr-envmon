#include "ds18b20.h"
#include "onewire.h"

bool ds18b20Init(void)
{
    if (oneWireReset())
    {
        return true;
    }
    else
    {
        return false;
    }
}

void ds18b20StartConversion(void)
{
    oneWireReset();
    oneWireWriteByte(0xCC);
    oneWireWriteByte(0x44);
}

int16_t ds18b20ReadRaw(void)
{
    oneWireReset();
    oneWireWriteByte(0xCC);
    oneWireWriteByte(0xBE);

    uint8_t lsb = oneWireReadByte();
    uint8_t msb = oneWireReadByte();

    int16_t rawTemperature = (int16_t)((msb << 8) | lsb);

    return rawTemperature;
}
