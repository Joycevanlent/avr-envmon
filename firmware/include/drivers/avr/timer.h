#ifndef TIMER_H
#define TIMER_H

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

#endif // TIMER_H