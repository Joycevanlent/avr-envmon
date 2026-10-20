#ifndef DRIVERS_AVR_TIMER_H
#define DRIVERS_AVR_TIMER_H

#include <stdint.h>

/** 
 * @brief Initialize the timer.
 */
void timerInit();

/** 
 * @brief Get the number of milliseconds since the program started.
 * 
 * 
 * @return The number of milliseconds.
 */
uint32_t timerMillis();

#endif // DRIVERS_AVR_TIMER_H