#include "envmon_machine.h"
#include "platform.h"
#include "serial.h"
#include "timer.h"
#include "power.h"

#include <stdbool.h>

int main(void)
{
    platformInit();
    envmonMachineInit();

    while(true)
    {
        uint32_t currentMillis = timerMillis();
        envmonMachineRun(currentMillis);

        powerSleepIdle();
    }

    return 0;
}