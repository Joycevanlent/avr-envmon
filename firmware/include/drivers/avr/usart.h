#ifndef USART_H
#define USART_H

#include <stdint.h>

/**
 * @brief Initializes the USART peripheral with the specified baud rate and frame format.
 */
void usartInit(void);

/**
 * @brief Transmits a single byte through the USART peripheral.
 * 
 * 
 * @param byte The byte to transmit
 */
void usartTransmit(uint8_t byte);

/**
 * @brief Transmits a string through the USART peripheral
 * 
 * 
 * @param str The string to transmit
 */
void usartTransmitString(const char *str);

/**
 * @brief Flushes the USART transmit buffer.
 */
void usartFlush(void);

#endif // USART_H