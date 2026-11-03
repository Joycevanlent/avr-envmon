#include "spi.h"

#include <avr/io.h>

void spi_init(void)
{
    // Configure SPI pins
    DDRB |= _BV(DDB3) | _BV(DDB5) | _BV(DDB2);
    DDRB &= ~_BV(DDB4);

    // Keep slave select inactive
    PORTB |= _BV(DDB2);

    // Enable SPI and configure as master
    // CPOL = 0, CPHA = 0, MSB first
    SPCR = _BV(SPE) | _BV(MSTR);

    // Set SPI clock to F_CPU / 128 (125 kHz at 16 MHz)
    spi_set_clock_divider(128);
}

uint8_t spi_transfer(uint8_t data)
{
    // Start transmission
    SPDR = data;

    // Wait for transmission complete
    while (!(SPSR & _BV(SPIF)))
    {
    }

    return SPDR;
}

void spi_set_clock_divider(uint8_t divider)
{
    // Clear clock divider bits
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
        // Unsupported divider: leave SPI at F_CPU / 4
        break;
    }
}

void spi_enable(void)
{
    SPCR |= _BV(SPE);
}

void spi_disable(void)
{
    SPCR &= ~_BV(SPE);
}