#include "envmon_machine.h"
#include "sensor_data.h"
#include "sensor_manager.h"
#include "drivers/avr/usart.h"
#include "drivers/avr/timer.h"

#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdio.h>

typedef enum
{
    STATE_SLEEP_IDLE,
    STATE_MEASURING,
    STATE_ERROR
} state;

static state currentState = STATE_SLEEP_IDLE;
static uint32_t previousMillis = 0;
static const uint32_t measureInterval = 2000;

void envmonMachineInit(void)
{
    usartInit();
    timerInit();

    sei();
}

void envmonMachineLoop(void)
{
    uint32_t currentMillis = timerMillis();
    switch (currentState)
    {
    case STATE_SLEEP_IDLE:
    {
        if (currentMillis - previousMillis >= measureInterval)
        {
            previousMillis += measureInterval;
            currentState = STATE_MEASURING;
        }
        break;
    }
    case STATE_MEASURING:
    {
        sensorManagerReadSensors();

        sensorPayload *currentPayload = sensorManagerGetData();

        char textBuffer[24];
        int length = snprintf(textBuffer, sizeof(textBuffer), "%d,%u\r\n",
                                  currentPayload->temperature,
                                  currentPayload->brightness);

        if (length > 0 && (uint8_t)length < sizeof(textBuffer))
        {
            if(!usartSend((const uint8_t *)textBuffer, (uint8_t)length))
            {
                // not send
            }
        }

        currentState = STATE_SLEEP_IDLE;
        break;
    }
    case STATE_ERROR:
    {
        break;
    }
    default:
        currentState = STATE_SLEEP_IDLE;
        break;
    }
}

void stateSleepIdleEntry()
{
}

void stateSleepIdleDo()
{
}

void stateSleepIdleExit()
{
}

void stateMeasuringEntry()
{
}

void stateMeasuringDo()
{
}

void stateMeasuringExit()
{
}

void stateErrorEntry()
{
}

void stateErrorDo()
{
}

void stateErrorExit()
{
}
