#include "gpio.h"

void gpioSetMode(const gpioPin *p, gpioMode mode)
{
    if (mode == GPIO_OUTPUT)
        *p->ddr |= p->mask;
    else
        *p->ddr &= ~p->mask;
}

void gpioWrite(const gpioPin *p, bool high)
{
    if (high)
        *p->port |= p->mask;
    else
        *p->port &= ~p->mask;
}

bool gpioRead(const gpioPin *p)
{
    return (*p->pin & p->mask) != 0;
}