#include "onewire.h"
#include "delay.h"
#include "board_config.h"

#include <avr/io.h>
#include <util/atomic.h>

#define GLUE(a, b) a##b
#define JOIN(a, b) GLUE(a, b)

#define ONEWIRE_DDR JOIN(DDR, ONEWIRE_PORT_LETTER)
#define ONEWIRE_PORT JOIN(PORT, ONEWIRE_PORT_LETTER)
#define ONEWIRE_PIN JOIN(PIN, ONEWIRE_PORT_LETTER)
#define ONEWIRE_BIT (1 << ONEWIRE_PIN_NUM)

static inline void oneWireDriveLow(void);
static inline void oneWireRelease(void);
static inline bool oneWireReadPin(void);

bool oneWireReset(void)
{
    oneWireDriveLow();
    delayUs(480);

    oneWireRelease();
    delayUs(70);

    bool presenceDetected = !oneWireReadPin();

    delayUs(410);

    return presenceDetected;
}

void oneWireWriteBit(bool bit)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        if (bit == 0)
        {
            oneWireDriveLow();
            delayUs(58);
            oneWireRelease();
            delayUs(2);
        }
        else
        {
            oneWireDriveLow();
            delayUs(6);
            oneWireRelease();
            delayUs(54);
        }
    }
}

bool oneWireReadBit(void)
{
    bool incomingBit = false;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        oneWireDriveLow();
        delayUs(3);

        oneWireRelease();
        delayUs(9);

        incomingBit = oneWireReadPin();
    }

    delayUs(50);

    return incomingBit;
}

void oneWireWriteByte(uint8_t byte)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        bool currentBit = byte & (1 << i);
        oneWireWriteBit(currentBit);
    }
}

uint8_t oneWireReadByte(void)
{
    uint8_t byte = 0;
    for (uint8_t i = 0; i < 8; i++)
    {
        bool bit = oneWireReadBit();

        if (bit == true)
        {
            byte |= (1 << i);
        }
    }

    return byte;
}

// Helpers
static inline void oneWireDriveLow(void)
{
    ONEWIRE_PORT &= ~ONEWIRE_BIT;
    ONEWIRE_DDR |= ONEWIRE_BIT; // Output
}

static inline void oneWireRelease(void)
{
    ONEWIRE_DDR &= ~ONEWIRE_BIT; // Input
}

static inline bool oneWireReadPin(void)
{
    if (ONEWIRE_PIN & ONEWIRE_BIT)
    {
        return true;
    }
    else
    {
        return false;
    }
}