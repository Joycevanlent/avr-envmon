#include "drivers/avr/usart.h"
#include "circularbuffer.h"

#define BAUD 9600

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/setbaud.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>


#define SEND_BUFFER_CAPACITY 32
_Static_assert(SEND_BUFFER_CAPACITY >= CB_MIN_CAPACITY && SEND_BUFFER_CAPACITY <= CB_MAX_CAPACITY, "Capacity must be between min and max");
static uint8_t sendDataStorage[SEND_BUFFER_CAPACITY];
static cbuffer sendBuffer;

void usartInit(void)
{
    cbInit(&sendBuffer, sendDataStorage, SEND_BUFFER_CAPACITY, sizeof(uint8_t), IGNORE_IF_FULL);

    // Set baud rate
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;

#if USE_2X
    UCSR0A |= (1 << U2X0);
#else
    UCSR0A &= ~(1 << U2X0);
#endif

    // Enable receiver and transmitter
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);

    // Set frame format: 8data, 2stop bit
    UCSR0C = (1 << USBS0) | (3 << UCSZ00);
}

bool usartSend(const uint8_t *data, uint8_t length)
{
    if (data == NULL || length == 0)
    {
        return false;
    }

    if (cbSpace(&sendBuffer) < length)
    {
        return false;
    }

    for (uint8_t i = 0; i < length; i++)
    {
        cbPush(&sendBuffer, &data[i]);
    }

    UCSR0B |= (1 << UDRIE0);

    return true;
}

void usartFlush(void)
{
    while (UCSR0A & (1 << RXC0))
    {
        (void)UDR0;
    }
}

ISR(USART_UDRE_vect)
{
    uint8_t byte = 0;
    if (cbPop(&sendBuffer, &byte))
    {
        UDR0 = byte;
    }

    if (cbCount(&sendBuffer) == 0)
    {
        UCSR0B &= ~(1 << UDRIE0);
    }
}