#include "envmon_machine.h"

#include <stdbool.h>

int main(void)
{
    envmonMachineInit();

    while(true)
    {
        envmonMachineLoop();
    }

    return 0;
}