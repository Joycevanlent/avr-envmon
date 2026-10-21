#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Initializes the USART peripheral with the specified baud rate and frame format.
 */
void serialInit(void);

/**
 * @brief Sends data over USART.
 */
bool serialSend(const uint8_t *data, uint8_t length);

/**
 * @brief Flushes the USART transmit buffer.
 */
void serialFlush(void);

#endif // SERIAL_H