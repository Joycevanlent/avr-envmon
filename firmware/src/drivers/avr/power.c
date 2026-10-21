#include "power.h"

#include <avr/sleep.h>
#include <avr/interrupt.h>

void powerSleepIdle(void)
{
    set_sleep_mode(SLEEP_MODE_IDLE);
    cli();

    sleep_enable();
    sei();
    sleep_cpu();
    sleep_disable();
}