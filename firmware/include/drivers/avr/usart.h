#ifndef USART_H
#define USART_H

#include <stdint.h>

/**
 * @brief Initializes the USART peripheral with the specified baud rate and frame format.
 */
void usartInit(void);

/**
 * @brief Sends data over USART.
 */
bool usartSend(const uint8_t *data, uint8_t length);

/**
 * @brief Flushes the USART transmit buffer.
 */
void usartFlush(void);

#endif // USART_H