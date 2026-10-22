/**
 * @file crc8.h
 * @brief Provides Dallas/Maxim CRC-8 calculation.
 *
 * Implements the CRC-8 algorithm used by Dallas/Maxim 1-Wire devices
 * for validating data such as sensor scratchpads and ROM codes.
 */

#ifndef CRC8_H
#define CRC8_H

#include <stddef.h>
#include <stdint.h>

/**
 * @brief Calculates the Dallas/Maxim 1-Wire CRC-8 checksum.
 *
 * Calculates a CRC-8 checksum over the specified byte array.
 *
 * @param[in] data Pointer to the input data.
 * @param[in] length Number of bytes to process.
 *
 * @return The calculated CRC-8 checksum. Returns 0 if data is NULL
 *         and length is greater than zero.
 */
uint8_t crc8Calculate(const uint8_t *data, size_t length);

#endif // CRC8_H