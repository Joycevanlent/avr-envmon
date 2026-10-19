#include "envmon_machine.h"
#include "sensor_data.h"
#include "sensor_manager.h"
#include "drivers/avr/usart.h"
#include "drivers/avr/timer.h"
#include "circularbuffer.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#define SEND_BUFFER_CAPACITY 16
static uint8_t sendDataStorage[SEND_BUFFER_CAPACITY];
static cbuffer sendBuffer;

typedef enum
{
    SLEEP,
    READ_STATION,
    SEND_DATA
} state;

state currentState = SLEEP;
uint32_t previousMillis = 0;
uint32_t measureInterval = 2000;

void envmonMachineInit()
{
    usartInit();
    timerInit();
    cbInit(&sendBuffer, sendDataStorage, SEND_BUFFER_CAPACITY, sizeof(uint8_t), IGNORE_IF_FULL);

    sei();
}

void envmonMachineLoop()
{
    uint32_t currentMillis = timerMillis();
    switch (currentState)
    {
    case SLEEP:
        if (currentMillis - previousMillis >= measureInterval)
        {
            previousMillis = currentMillis;
            currentState = READ_STATION;
        }
        break;
    case READ_STATION:
        sensorManagerReadSensors();

        sensorPayload *currentPayload = sensorManagerGetData();

        char textBuffer[24];
        int length = snprintf(textBuffer, sizeof(textBuffer), "%d,%u\r\n",
                              currentPayload->temperature,
                              currentPayload->brightness);

        if (length > 0)
        {
            for (int i = 0; i < length; i++)
            {
                cbPush(&sendBuffer, &textBuffer[i]);
            }
        }

        currentState = SEND_DATA;
        break;
    case SEND_DATA:
        uint8_t tempByte = 0;

        while (cbPop(&sendBuffer, &tempByte))
        {
            usartTransmit(tempByte);
        }

        currentState = SLEEP;
        break;
    default:
        currentState = SLEEP;
        break;
    }
}
