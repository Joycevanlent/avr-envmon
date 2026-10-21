#include "adc.h"

#include <avr/io.h>
#include <stdint.h>

void adcInit(uint8_t channel)
{
    // Empty REFS bits and set reference to AVCC
    ADMUX = (ADMUX & ~(_BV(REFS1) | _BV(REFS0))) | _BV(REFS0);

    // Aign right
    ADMUX &= ~_BV(ADLAR);

    // Set prescaler to 128 (voor 16MHz clock -> 125kHz ADC clock)
    ADCSRA |= _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);

    // Turn on ADC
    ADCSRA |= _BV(ADEN);

    // Throw away first measurement due to inaccuracy
    (void)adcRead(channel);
}

uint16_t adcRead(uint8_t channel)
{
    if (channel > 5)
    {
        return 0; // Invalid channel (A0 up to and including A5)
    }

    // Clear only the lower 4 MUX bits (MUX3:0), keeping reference settings intact
    ADMUX &= 0xF0;

    // Set the new analog channel
    ADMUX |= (channel & 0x0F);

    // Start measuring
    ADCSRA |= _BV(ADSC);

    // Wait for measurement to complete
    while (ADCSRA & (1 << ADSC))
    {
    }

    uint16_t adcValue = ADC;

    return adcValue;
}