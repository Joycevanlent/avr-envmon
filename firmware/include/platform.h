 /**
 * @file platform.h
 * @brief Provides platform initialization for the firmware.
 *
 * Initializes the platform peripherals required by the application,
 * including serial communication and the system timer.
 */

#ifndef PLATFORM_H
#define PLATFORM_H

/**
 * @brief Initializes the platform peripherals.
 *
 * Initializes the serial interface and system timer, then enables
 * global interrupts.
 *
 * Call this function once during application startup, before
 * entering the main application loop.
 */
void platformInit(void);

#endif // PLATFORM_H
