
/**
 * @file envmon_machine.h
 * @brief Provides the interface for the environmental monitor state machine.
 *
 * The state machine coordinates sensor measurements, processes sensor
 * results, and transmits measurement data or error messages over serial.
 */

#ifndef ENVMON_MACHINE_H
#define ENVMON_MACHINE_H

#include <stdint.h>


/**
 * @brief Initializes the environmental monitor state machine.
 *
 * Initializes the sensor manager and sets the initial state based on
 * whether sensor initialization succeeds. If initialization fails,
 * the state machine enters the error state.
 */
void envmonMachineInit(void);

/**
 * @brief Executes one iteration of the state machine.
 *
 * Updates the current time and executes the run action associated with
 * the current state. Call this function regularly from the main loop
 * to keep measurements and state transitions progressing.
 *
 * @param[in] nowMillis Current system time in milliseconds.
 *                      The value may wrap around when the timer overflows.
 */
void envmonMachineRun(uint32_t nowMillis);

#endif // ENVMON_MACHINE_H
