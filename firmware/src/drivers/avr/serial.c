#include "serial.h"
#include "circularbuffer.h"

#ifndef BAUD
    #define BAUD 9600
#endif

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/setbaud.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#ifndef SEND_BUFFER_CAPACITY
    #define SEND_BUFFER_CAPACITY 32
#endif

_Static_assert(SEND_BUFFER_CAPACITY >= CB_MIN_CAPACITY && SEND_BUFFER_CAPACITY <= CB_MAX_CAPACITY, "Capacity must be between min and max");

static uint8_t sendDataStorage[SEND_BUFFER_CAPACITY];
static cbuffer sendBuffer;

void serialInit(void)
{
    cbInit(&sendBuffer, sendDataStorage, SEND_BUFFER_CAPACITY, sizeof(uint8_t), IGNORE_IF_FULL);

    // Set baud rate
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;

#if USE_2X
    UCSR0A |= _BV(U2X0);
#else
    UCSR0A &= ~_BV(U2X0);
#endif

    // Enable receiver and transmitter
    UCSR0B = _BV(RXEN0) | _BV(TXEN0);

    // Set frame format: 8 data bits, 2 stop bits
    UCSR0C = _BV(USBS0) | _BV(UCSZ01) | _BV(UCSZ00);
}

bool serialSend(const uint8_t *data, uint8_t length)
{
    if (data == NULL || length == 0 || cbSpace(&sendBuffer) < length)
    {
        return false;
    }

    for (uint8_t i = 0; i < length; i++)
    {
        cbPush(&sendBuffer, &data[i]);
    }

    // Enable Data Register Empty Interrupt to trigger transmission
    UCSR0B |= _BV(UDRIE0);

    return true;
}

void serialFlush(void)
{
    while (UCSR0A & _BV(RXC0))
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

    // Disable interrupt when the buffer is empty
    if (cbCount(&sendBuffer) == 0)
    {
        UCSR0B &= ~_BV(UDRIE0);
    }
}