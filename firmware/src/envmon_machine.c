#include "envmon_machine.h"
#include "sensor_manager.h"
#include "sensor_status.h"
#include "protocol.h"
#include "serial.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>

#define MAX_CONSECUTIVE_ERRORS 3U

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
static void registerError(void);

static envmonState currentState = ENVMON_STATE_IDLE;
static uint32_t lastNowMillis = 0;
static uint32_t stateEnteredMillis = 0;

static const uint32_t errorTimeoutMillis = 5000U;
static uint8_t consecutiveErrors = 0;
static bool permanentError = false;

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
    lastNowMillis = 0;
    stateEnteredMillis = 0;
    consecutiveErrors = 0;
    permanentError = false;
    currentState = ENVMON_STATE_IDLE;

    sensorStatus initStatus = sensorManagerInit();

    if (initStatus != SENSOR_OK)
    {
        registerError();
    }
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
        previousMillis = lastNowMillis;
        changeState(ENVMON_STATE_MEASURING);
    }
}

// -- ENVMON_STATE_MEASURING --
static void measuringOnEnter(void)
{
    sensorStatus status = sensorManagerStartMeasurement();
    if (status != SENSOR_OK)
    {
        registerError();
    }
}

static void measuringOnRun(void)
{
    bool dataReady = false;
    sensorStatus status = sensorManagerIsDataReady(lastNowMillis, &dataReady);

    if (status != SENSOR_OK)
    {
        registerError();
        return;
    }

    if (dataReady)
    {
        consecutiveErrors = 0;

        const sensorPayload *currentPayload = sensorManagerGetData();

        char textBuffer[40];

        if (protocolFormatSensorPayload(
                textBuffer,
                sizeof(textBuffer),
                currentPayload))
        {
            serialSend(
                (const uint8_t *)textBuffer,
                (uint8_t)strlen(textBuffer));
        }

        changeState(ENVMON_STATE_IDLE);
    }
}

// --- ENVMON_STATE_ERROR ---
static void errorOnEnter(void)
{
    char textBuffer[24];

    if (protocolFormatSensorError(textBuffer, sizeof(textBuffer)))
    {
        serialSend(
            (const uint8_t *)textBuffer,
            (uint8_t)strlen(textBuffer));
    }
}

static void errorOnRun(void)
{
    if (permanentError)
    {
        return;
    }

    if (stateElapsedMillis() >= errorTimeoutMillis)
    {
        changeState(ENVMON_STATE_IDLE);
    }
}

static void registerError(void)
{
    if (consecutiveErrors < MAX_CONSECUTIVE_ERRORS)
    {
        consecutiveErrors++;
    }

    if (consecutiveErrors >= MAX_CONSECUTIVE_ERRORS)
    {
        permanentError = true;
    }

    changeState(ENVMON_STATE_ERROR);
}

static uint32_t stateElapsedMillis(void)
{
    uint32_t elapsedMillis = lastNowMillis - stateEnteredMillis;
    return elapsedMillis;
}
