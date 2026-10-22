
/**
 * @file onewire.h
 * @brief Provides low-level communication over the 1-Wire bus.
 *
 * Implements basic 1-Wire bus operations, including reset and presence
 * detection, bit transmission, and byte transmission.
 *
 * The implementation uses the AVR hardware configuration defined in
 * board_config.h.
 */

#ifndef ONEWIRE_H
#define ONEWIRE_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Resets the 1-Wire bus and checks for a presence pulse.
 *
 * Drives the bus low for the reset interval, releases it, and checks
 * whether a connected device responds with a presence pulse.
 *
 * @return true if a presence pulse is detected; false otherwise.
 */
bool oneWireReset(void);

/**
 * @brief Writes one bit to the 1-Wire bus.
 *
 * @param[in] bit Bit value to transmit: false for 0 or true for 1.
 */
void oneWireWriteBit(bool bit);

/**
 * @brief Reads one bit from the 1-Wire bus.
 *
 * @return The bit read from the bus: true for 1 or false for 0.
 */
bool oneWireReadBit(void);

/**
 * @brief Writes one byte to the 1-Wire bus.
 *
 * Transmits the byte least-significant bit first.
 *
 * @param[in] byte Byte value to transmit.
 */
void oneWireWriteByte(uint8_t byte);

/**
 * @brief Reads one byte from the 1-Wire bus.
 *
 * Reads eight bits, least-significant bit first, and combines them
 * into a byte.
 *
 * @return The byte read from the bus.
 */
uint8_t oneWireReadByte(void);

#endif // ONEWIRE_H
