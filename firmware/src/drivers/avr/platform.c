#include "serial.h"
#include "timer.h"

#include <avr/io.h>
#include <avr/interrupt.h>

void platformInit(void)
{
    serialInit();
    timerInit();
    sei();
}