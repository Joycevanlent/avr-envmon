#ifndef ENVMON_MACHINE_H
#define ENVMON_MACHINE_H

#include <stdint.h>

/**
 * @brief Initializes the environmental monitor machine.
 */
void envmonMachineInit(void);

/**
 * @brief Execution loop for the environmental monitor machine.
 */
void envmonMachineLoop(uint32_t nowMillis);

#endif // ENVMON_MACHINE_H
