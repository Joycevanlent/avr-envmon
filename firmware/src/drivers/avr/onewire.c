#include "onewire.h"

#include <avr/io.h>
#include <util/delay.h>
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
    _delay_us(480);

    oneWireRelease();
    _delay_us(70);

    bool presenceDetected = !oneWireReadPin();

    _delay_us(410);

    return presenceDetected;
}

void oneWireWriteBit(bool bit)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        if (bit == 0)
        {
            oneWireDriveLow();
            _delay_us(58);
            oneWireRelease();
            _delay_us(2);
        }
        else
        {
            oneWireDriveLow();
            _delay_us(6);
            oneWireRelease();
            _delay_us(54);
        }
    }
}

bool oneWireReadBit(void)
{
    bool incomingBit = false;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        oneWireDriveLow();
        _delay_us(3);

        oneWireRelease();
        _delay_us(9);

        incomingBit = oneWireReadPin();
    }

    _delay_us(50);

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
    ONEWIRE_PORT |= ONEWIRE_BIT;
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