#include "delay.h"

#include <util/delay.h>

void delayUs(uint16_t microseconds)
{
    while (microseconds--)
    {
        _delay_us(1);
    }
}

void delayMs(uint16_t milliseconds)
{
    while (milliseconds--)
    {
        _delay_ms(1);
    }
}