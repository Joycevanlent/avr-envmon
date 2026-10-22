/**
 * @file board_config.h
 * @brief Defines hardware and peripheral configuration for the target board.
 *
 * Centralizes the board-specific pin assignments and peripheral settings
 * used by the environmental monitor firmware.
 *
 * Current target: Arduino Uno / ATmega328P.
 */

#ifndef BOARD_CONFIG_H
#define BOARD_CONFIG_H

/* --- 1-Wire temperature sensor --- */

/** @brief AVR port letter used by the 1-Wire bus. */
#define ONEWIRE_PORT_LETTER D

/** @brief Bit position of the 1-Wire data pin on the selected port. */
#define ONEWIRE_PIN_NUM 7

/* --- Light sensor --- */

/** @brief ADC channel connected to the light-dependent resistor (LDR). */
#define LDR_CHANNEL 0

/* --- UART serial communication --- */

/** @brief UART baud rate in bits per second. */
#define SERIAL_BAUD 9600

/** @brief Capacity of the serial transmit buffer in bytes. */
#define SEND_BUFFER_CAPACITY 32

#endif // BOARD_CONFIG_H
