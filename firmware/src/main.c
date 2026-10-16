#include "drivers/avr/usart.h"
#include "drivers/avr/timer.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdlib.h>
#include <stdbool.h>

int main(void)
{
    usartInit();
    timerInit();
    sei();

    uint32_t previousMillis = 0;
    uint32_t interval = 2000;
    while (true)
    {
        uint32_t currentMillis = timerMillis();
        if(currentMillis - previousMillis >= interval)
        {
            previousMillis = currentMillis;

            uint32_t seconds = currentMillis / 1000;
            char buffer[12];

            ultoa(seconds, buffer, 10);

            usartTransmitString("Uptime: ");
            usartTransmitString(buffer);
            usartTransmitString(" seconds\n\r");
        }
    }

    return 0;
}