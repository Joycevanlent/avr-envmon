#include "envmon_machine.h"
#include "sensor_data.h"
#include "sensor_manager.h"
#include "usart.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef enum
{
    STATE_IDLE,
    STATE_MEASURING,
    STATE_ERROR,
    STATE_COUNT // For static assert
} state;

typedef void (*stateAction)(void);
typedef void (*stateRunAction)(uint32_t currentMillis);

typedef struct
{
    stateAction onEnter;
    stateRunAction onRun;
    stateAction onExit;
} stateBehavior;

void changeState(state newState);
static void idleOnRun(uint32_t currentMillis);
static void measuringOnEnter(void);
static void errorOnEnter(void);
static void errorOnRun(uint32_t nowMillis);
static bool formatSensorPayload(char *buffer, uint8_t bufferSize, const sensorPayload *payload);

static state currentState = STATE_IDLE;
//static uint32_t errorTimestamp = 0;
static uint16_t sendErrorCount = 0;

static const stateBehavior stateTable[STATE_COUNT] = {
    [STATE_IDLE] = {
        .onEnter = NULL,
        .onRun = idleOnRun,
        .onExit = NULL},
    [STATE_MEASURING] = {.onEnter = measuringOnEnter, .onRun = NULL, .onExit = NULL},
    [STATE_ERROR] = {.onEnter = errorOnEnter, .onRun = errorOnRun, .onExit = NULL}};

_Static_assert(sizeof(stateTable) / sizeof(stateTable[0]) == STATE_COUNT, "State table size mismatch");

void envmonMachineInit(void)
{
    sensorManagerInit();
    currentState = STATE_IDLE;
}

void changeState(state newState)
{
    if (newState >= STATE_COUNT)
    {
        return;
    }

    if (stateTable[currentState].onExit != NULL)
    {
        stateTable[currentState].onExit();
    }

    currentState = newState;

    if (stateTable[currentState].onEnter != NULL)
    {
        stateTable[currentState].onEnter();
    }
}

void envmonMachineLoop(uint32_t nowMillis)
{
    if (currentState < STATE_COUNT && stateTable[currentState].onRun != NULL)
    {
        stateTable[currentState].onRun(nowMillis);
    }
}

// temp helper
static bool formatSensorPayload(char *buffer, uint8_t bufferSize, const sensorPayload *payload)
{
    int length = snprintf(buffer, bufferSize, "%d,%u\r\n", payload->temperature, payload->brightness);
    return (length > 0 && (uint8_t)length < bufferSize);
}

// -- STATE_SLEEP_IDLE --
static void idleOnRun(uint32_t currentMillis)
{
    static uint32_t previousMillis = 0;
    const uint32_t measureInterval = 2000;

    if (currentMillis - previousMillis >= measureInterval)
    {
        previousMillis += measureInterval;
        changeState(STATE_MEASURING);
    }
}

// -- STATE_MEASURING --
static void measuringOnEnter(void)
{
    sensorManagerReadSensors();
    sensorPayload *currentPayload = sensorManagerGetData();

    char textBuffer[24];
    if (formatSensorPayload(textBuffer, sizeof(textBuffer), currentPayload))
    {
        if (!usartSend((const uint8_t *)textBuffer, (uint8_t)strlen(textBuffer)))
        {
            sendErrorCount++;
            changeState(STATE_ERROR);
            return;
        }
    }

    changeState(STATE_IDLE);
}

// --- STATE_ERROR ---
static void errorOnEnter(void)
{
}

static void errorOnRun(uint32_t nowMillis)
{
    static uint32_t errorStartMillis = 0;
    static bool timerStarted = false;
    const uint32_t errorTimeout = 5000;

    if (!timerStarted)
    {
        errorStartMillis = nowMillis;
        timerStarted = true;
    }

    if (nowMillis - errorStartMillis >= errorTimeout)
    {
        timerStarted = false;
        changeState(STATE_IDLE);
    }
}