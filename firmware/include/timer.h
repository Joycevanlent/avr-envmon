/**
 * @file timer.h
 * @brief Provides a millisecond system timer for the AVR platform.
 *
 * Uses Timer0 in Clear Timer on Compare Match (CTC) mode to maintain
 * a millisecond counter. The counter is updated by a timer interrupt
 * and can be read safely from the main application.
 */

#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

/**
 * @brief Initializes the system timer.
 *
 * Configures Timer0 in CTC mode with a prescaler of 64, sets the
 * compare-match value, and enables the Compare Match A interrupt.
 *
 * The timer configuration is intended to generate an interrupt
 * every millisecond when the MCU clock frequency matches the
 * configuration used by the firmware.
 */
void timerInit(void);

/**
 * @brief Returns the elapsed system time in milliseconds.
 *
 * Reads the millisecond counter atomically to prevent inconsistent
 * values when the counter is updated by the timer interrupt.
 *
 * @return Elapsed milliseconds since timer initialization.
 *         The counter wraps around after reaching UINT32_MAX.
 */
uint32_t timerMillis(void);

#endif // TIMER_H
