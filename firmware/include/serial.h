/**
 * @file serial.h
 * @brief Provides UART serial communication for the AVR platform.
 *
 * Provides functions to initialize UART communication, queue data for
 * transmission, and flush received data. Transmission is handled
 * asynchronously using a transmit buffer and the UART data-register-empty
 * interrupt.
 */

#ifndef SERIAL_H
#define SERIAL_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Initializes the UART serial interface.
 *
 * Initializes the transmit buffer, configures the baud rate, enables
 * the UART transmitter and receiver, and configures the frame format.
 *
 * The default baud rate is 9600 if BAUD is not defined.
 */
void serialInit(void);

/**
 * @brief Queues data for serial transmission.
 *
 * Copies the supplied bytes into the transmit buffer. Actual transmission
 * is performed asynchronously by the UART interrupt handler.
 *
 * @param[in] data Pointer to the bytes to transmit.
 * @param[in] length Number of bytes to queue.
 *
 * @return true if all bytes were queued successfully; false if the
 *         pointer is NULL, the length is zero, or the buffer lacks space.
 */
bool serialSend(const uint8_t *data, uint8_t length);

/**
 * @brief Discards any currently received UART data.
 *
 * Reads and discards bytes currently available in the UART receive
 * register. This function does not wait for queued transmit data
 * to finish sending.
 */
void serialFlush(void);

#endif // SERIAL_H
