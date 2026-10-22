/**
 * @file power.h
 * @brief Provides power-saving functionality for the firmware.
 *
 * Provides functions to reduce power consumption by placing the AVR
 * microcontroller in a low-power sleep mode.
 */

#ifndef POWER_H
#define POWER_H

/**
 * @brief Puts the microcontroller into idle sleep mode.
 *
 * Configures the AVR for idle sleep, enables sleep mode, and allows
 * interrupts to wake the microcontroller. Execution resumes after
 * the microcontroller wakes up.
 *
 * Peripherals that remain enabled may continue operating during
 * idle sleep.
 */
void powerSleepIdle(void);

#endif // POWER_H
