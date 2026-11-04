/**
 * @file delay.h
 * @brief Platform-independent interface for blocking delays.
 *
 * Provides functions to delay program execution for a specified
 * number of microseconds or milliseconds.
 *
 * The actual delay implementation is platform-dependent.
 */

#ifndef DELAY_H
#define DELAY_H

#include <stdint.h>

/**
 * @brief Block execution for a specified number of microseconds.
 *
 * @param microseconds Duration of the delay in microseconds.
 *
 * @note This function blocks execution until the delay has elapsed.
 *       Actual timing depends on the platform implementation.
 */
void delayUs(uint16_t microseconds);


/**
 * @brief Block execution for a specified number of milliseconds.
 *
 * @param milliseconds Duration of the delay in milliseconds.
 *
 * @note This function blocks execution until the delay has elapsed.
 *       Actual timing depends on the platform implementation.
 */
void delayMs(uint16_t milliseconds);

#endif