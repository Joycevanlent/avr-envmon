/**
 * @file spi.h
 * @brief Hardware SPI driver for the ATmega328P.
 *
 * Provides SPI master initialization, data transfer
 * and clock configuration.
 */

#ifndef SPI_H
#define SPI_H

#include <stdint.h>

/**
 * @brief Initializes the SPI peripheral in master mode.
 *
 * Configures the SPI pins, enables the peripheral
 * and sets the default clock divider to 128.
 */
void spiInit(void);

/**
 * @brief Transfers one byte over SPI.
 *
 * @param data Byte to transmit.
 * @return Byte received during the transfer.
 */
uint8_t spiTransfer(uint8_t data);

/**
 * @brief Sets the SPI clock divider.
 *
 * @param divider Clock divider. Supported values are
 *                2, 4, 8, 16, 32, 64 and 128.
 *                Invalid values default to 4.
 */
void spiSetClockDivider(uint8_t divider);

/**
 * @brief Enables the SPI peripheral.
 */
void spiEnable(void);

/**
 * @brief Disables the SPI peripheral.
 */
void spiDisable(void);

#endif /* SPI_H */