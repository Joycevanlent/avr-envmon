#include "envmon_machine.h"
#include "sensor_data.h"
#include "sensor_manager.h"
#include "serial.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>

typedef enum
{
    ENVMON_STATE_IDLE,
    ENVMON_STATE_MEASURING,
    ENVMON_STATE_ERROR,
    ENVMON_STATE_COUNT // For static assert
} envmonState;

typedef void (*stateAction)(void);

typedef struct
{
    stateAction onEnter;
    stateAction onRun;
    stateAction onExit;
} stateBehavior;

static void changeState(envmonState newState);
static void idleOnRun(void);
static void measuringOnEnter(void);
static void measuringOnRun(void);
static void errorOnEnter(void);
static void errorOnRun(void);
static bool formatSensorPayload(char *buffer, uint8_t bufferSize, const sensorPayload *payload);
static uint32_t stateElapsedMillis(void);

static uint32_t lastNowMillis = 0;
static uint32_t stateEnteredMillis = 0;

static envmonState currentState = ENVMON_STATE_IDLE;
// static uint32_t errorTimestamp = 0;
static uint16_t sendErrorCount = 0;

static const stateBehavior stateTable[ENVMON_STATE_COUNT] = {
    [ENVMON_STATE_IDLE] = {
        .onEnter = NULL,
        .onRun = idleOnRun,
        .onExit = NULL},
    [ENVMON_STATE_MEASURING] = {.onEnter = measuringOnEnter, .onRun = measuringOnRun, .onExit = NULL},
    [ENVMON_STATE_ERROR] = {.onEnter = errorOnEnter, .onRun = errorOnRun, .onExit = NULL}};

_Static_assert(sizeof(stateTable) / sizeof(stateTable[0]) == ENVMON_STATE_COUNT, "State table size mismatch");

void envmonMachineInit(void)
{
    sensorManagerInit();
    lastNowMillis = 0;
    stateEnteredMillis = 0;
    currentState = ENVMON_STATE_IDLE;
}

static void changeState(envmonState newState)
{
    if (newState >= ENVMON_STATE_COUNT)
    {
        return;
    }

    stateEnteredMillis = lastNowMillis;

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

void envmonMachineRun(uint32_t nowMillis)
{
    lastNowMillis = nowMillis;
    if (currentState < ENVMON_STATE_COUNT && stateTable[currentState].onRun != NULL)
    {
        stateTable[currentState].onRun();
    }
}

// -- ENVMON_STATE_IDLE --
static void idleOnRun(void)
{
    static uint32_t previousMillis = 0;
    const uint32_t measureInterval = 2000;

    if (lastNowMillis - previousMillis >= measureInterval)
    {
        previousMillis += measureInterval;
        changeState(ENVMON_STATE_MEASURING);
    }
}

// -- ENVMON_STATE_MEASURING --
static void measuringOnEnter(void)
{
    sensorManagerReadSensors();
}

static void measuringOnRun(void)
{
    const sensorPayload *currentPayload = sensorManagerGetData();

    char textBuffer[24];
    if (formatSensorPayload(textBuffer, sizeof(textBuffer), currentPayload))
    {
        if (!serialSend((const uint8_t *)textBuffer, (uint8_t)strlen(textBuffer)))
        {
            sendErrorCount++;
            changeState(ENVMON_STATE_IDLE);
            return;
        }
    }

    changeState(ENVMON_STATE_IDLE);
}

// --- ENVMON_STATE_ERROR ---
static void errorOnEnter(void)
{
    // stub
}

static void errorOnRun(void)
{
    changeState(ENVMON_STATE_IDLE);
}

// temp helper
static bool formatSensorPayload(char *buffer, uint8_t bufferSize, const sensorPayload *payload)
{
    int length = snprintf(buffer, bufferSize, "%d,%u\r\n", payload->temperature, payload->brightness);
    return (length > 0 && length < (int)bufferSize);
}

__attribute__((unused)) uint32_t stateElapsedMillis(void)
{
    uint32_t elapsedMillis = lastNowMillis - stateEnteredMillis;
    return elapsedMillis;
}