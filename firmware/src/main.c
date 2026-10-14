#include "usart.h"
#include <util/delay.h>
#include <stdbool.h>
#include <avr/io.h>

int main(void)
{
    usartInit();
    while (true)
    {
        usartTransmitString("hello\r\n");
        _delay_ms(500);
    }

    return 0;
}