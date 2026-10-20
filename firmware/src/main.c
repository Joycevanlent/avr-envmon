#include "envmon_machine.h"
#include "usart.h"
#include "timer.h"
#include "power.h"

#include <stdbool.h>

int main(void)
{
    usartInit();
    timerInit();
    envmonMachineInit();

    while(true)
    {
        uint32_t currentMillis = timerMillis();
        envmonMachineLoop(currentMillis);

        powerSleepIdle();
    }

    return 0;
}