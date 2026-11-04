
#include "spi.h"

#include <avr/io.h>

void spiInit(void)
{
    // Configure MOSI, SCK and hardware SS as outputs
    DDRB |= _BV(DDB3) | _BV(DDB5) | _BV(DDB2);

    // Configure MISO as input
    DDRB &= ~_BV(DDB4);

    // Keep hardware SS high
    PORTB |= _BV(PORTB2);

    // Enable SPI, master mode, mode 0, MSB first
    // Clock divider is configured separately
    SPCR = _BV(SPE) | _BV(MSTR);

    spiSetClockDivider(128);
}

uint8_t spiTransfer(uint8_t data)
{
    SPDR = data;

    while (!(SPSR & _BV(SPIF)))
    {
    }

    return SPDR;
}

void spiSetClockDivider(uint8_t divider)
{
    SPCR &= ~(_BV(SPR1) | _BV(SPR0));
    SPSR &= ~_BV(SPI2X);

    switch (divider)
    {
    case 2:
        SPSR |= _BV(SPI2X);
        break;

    case 4:
        break;

    case 8:
        SPSR |= _BV(SPI2X);
        SPCR |= _BV(SPR0);
        break;

    case 16:
        SPCR |= _BV(SPR0);
        break;

    case 32:
        SPSR |= _BV(SPI2X);
        SPCR |= _BV(SPR1);
        break;

    case 64:
        SPSR |= _BV(SPI2X);
        SPCR |= _BV(SPR1) | _BV(SPR0);
        break;

    case 128:
        SPCR |= _BV(SPR1) | _BV(SPR0);
        break;

    default:
        // Unsupported divider: retain F_CPU / 4.
        break;
    }
}

void spiEnable(void)
{
    SPCR |= _BV(SPE);
}

void spiDisable(void)
{
    SPCR &= ~_BV(SPE);
}