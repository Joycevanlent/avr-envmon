#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <stdint.h>

// Volatile due to being modified in an ISR
static volatile uint32_t millisCounter = 0;

void timerInit()
{
    // Choose mode
    TCCR0A |= _BV(WGM01);

    // Set compare value in the OCR0A register
    OCR0A = 249;

    // Compare match interrupt enable
    TIMSK0 |= _BV(OCIE0A);

    // Set prescaler to 64 (249 fits into 8 bit)
    TCCR0B &= ~_BV(CS02);
    TCCR0B |= _BV(CS01);
    TCCR0B |= _BV(CS00);
}

/**
 * @brief Timer0 Compare Match A interrupt service routine.
 * This ISR is called every time Timer0 reaches the value in OCR0A.
 */
ISR(TIMER0_COMPA_vect)
{
    millisCounter++;
}

uint32_t timerMillis()
{
    uint32_t millis;

    // Using ATOMIC_BLOCK to ensure that the read operation is not interrupted
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        millis = millisCounter;
    }

    return millis;
}