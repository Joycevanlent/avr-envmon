#include "usart.h"
#include <avr/io.h>
#include <stdbool.h>
#include <stdint.h>

//#define F_CPU 16000000UL
#define BAUD 9600

#include <util/setbaud.h>

void usartInit(void)
{
    /*Set baud rate */
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;
    #if USE_2X
        UCSR0A |= (1 << U2X0);
    #else
        UCSR0A &= ~(1 << U2X0);
    #endif
    /* Enable receiver and transmitter */
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    /* Set frame format: 8data, 2stop bit */
    UCSR0C = (1 << USBS0) | (3 << UCSZ00);
}

void usartTransmit(uint8_t byte)
{
    // Wait for empty transmit buffer
    while (!(UCSR0A & (1 << UDRE0)))
    {
    }
    UDR0 = byte;
}

void usartTransmitString(const char *str)
{
    for (const char *p = str; *p != '\0'; p++)
    {
        usartTransmit(*p);
    }
}

void usartFlush(void)
{
    uint8_t dummy;
    while (UCSR0A & (1 << RXC0))
    {
        dummy = UDR0;
    }
}