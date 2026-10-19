#include "circularbuffer.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/// ASSERT(condition) checks if the condition is met, and if not,
/// aborts with an error message indicating the module and line where
/// the error occurred.
#define ASSERT(expr)                                                                     \
    do                                                                                  \
    {                                                                                   \
        if (!(expr))                                                                    \
        {                                                                               \
            fprintf(stderr, "\n%s:%d: CHECK failed: %s\n", __FILE__, __LINE__, #expr); \
            exit(EXIT_FAILURE);                                                         \
        }                                                                               \
    } while (0)

#define RUN_TEST(test_fn)                  \
    do                                     \
    {                                      \
        printf("Running %s...", #test_fn); \
        test_fn();                         \
        printf(" [PASS]\n");               \
    } while (0)

#define CAP_2 2U
#define CAP_3 3U
#define CAP_4 4U

static uint8_t storage_u8[CB_MAX_CAPACITY];
static uint32_t storage_u32[CAP_3];

typedef struct
{
    int16_t temperature;
    uint16_t light;
    uint32_t timestamp;
} reading;


static void test_cb_init(void)
{
    cbuffer buffer;
 
    ASSERT(cbInit(&buffer, storage_u8, CAP_4, sizeof(uint8_t), IGNORE_IF_FULL));
    ASSERT(buffer.data == storage_u8);
    ASSERT(buffer.capacity == CAP_4);
    ASSERT(buffer.elementSize == sizeof(uint8_t));
    ASSERT(buffer.mode == IGNORE_IF_FULL);
    ASSERT(buffer.head == 0);
    ASSERT(buffer.tail == 0);
    ASSERT(buffer.dropped == 0);
 
    /* The limits themselves are valid */
    ASSERT(cbInit(&buffer, storage_u8, CB_MIN_CAPACITY, 1, IGNORE_IF_FULL));
    ASSERT(cbInit(&buffer, storage_u8, CB_MAX_CAPACITY, 1, OVERWRITE_IF_FULL));
}

static void test_cb_init_rejects_bad_arguments(void)
{
    cbuffer buffer;
 
    ASSERT(!cbInit(NULL, storage_u8, CAP_4, 1, IGNORE_IF_FULL));
    ASSERT(!cbInit(&buffer, NULL, CAP_4, 1, IGNORE_IF_FULL));
    ASSERT(!cbInit(&buffer, storage_u8, 0, 1, IGNORE_IF_FULL));
    ASSERT(!cbInit(&buffer, storage_u8, 1, 1, IGNORE_IF_FULL)); /* could hold nothing */
    ASSERT(!cbInit(&buffer, storage_u8, CB_MAX_CAPACITY + 1U, 1, IGNORE_IF_FULL));
    ASSERT(!cbInit(&buffer, storage_u8, CAP_4, 0, IGNORE_IF_FULL));
    ASSERT(!cbInit(&buffer, storage_u8, CAP_4, 1, (enum cbmode)42));
}

static void test_cb_push_pop_basic(void)
{
    cbuffer buffer;
    ASSERT(cbInit(&buffer, (uint8_t *)storage_u32, CAP_3, sizeof(uint32_t), IGNORE_IF_FULL));
 
    uint32_t send1 = 42;
    uint32_t send2 = 84;
    uint32_t recv = 0;
 
    ASSERT(!cbPop(&buffer, &recv));
    ASSERT(cbPush(&buffer, &send1));
    ASSERT(cbPush(&buffer, &send2));
    ASSERT(cbPop(&buffer, &recv));
    ASSERT(recv == 42);
    ASSERT(cbPop(&buffer, &recv));
    ASSERT(recv == 84);
    ASSERT(!cbPop(&buffer, &recv));
}

static void test_cb_counts_and_spaces(void)
{
    cbuffer buffer;
    ASSERT(cbInit(&buffer, storage_u8, CAP_4, sizeof(uint8_t), IGNORE_IF_FULL));
 
    ASSERT(cbCount(&buffer) == 0);
    ASSERT(cbSpace(&buffer) == 3); /* usable capacity is capacity - 1 */
 
    uint8_t val = 10;
    ASSERT(cbPush(&buffer, &val));
    ASSERT(cbCount(&buffer) == 1);
    ASSERT(cbSpace(&buffer) == 2);
 
    ASSERT(cbPush(&buffer, &val));
    ASSERT(cbPush(&buffer, &val));
    ASSERT(cbCount(&buffer) == 3);
    ASSERT(cbSpace(&buffer) == 0);
}

static void test_cb_ignore_if_full(void)
{
    cbuffer buffer;
    ASSERT(cbInit(&buffer, storage_u8, CAP_3, sizeof(uint8_t), IGNORE_IF_FULL));
 
    uint8_t val1 = 11, val2 = 22, val3 = 33, recv = 0;
 
    ASSERT(cbPush(&buffer, &val1));
    ASSERT(cbPush(&buffer, &val2));
    ASSERT(!cbPush(&buffer, &val3));
    ASSERT(cbDropped(&buffer) == 1);
    ASSERT(cbCount(&buffer) == 2);
 
    ASSERT(cbPop(&buffer, &recv));
    ASSERT(recv == 11);
 
    /* Room again after a pop */
    ASSERT(cbPush(&buffer, &val3));
    ASSERT(cbPop(&buffer, &recv));
    ASSERT(recv == 22);
    ASSERT(cbPop(&buffer, &recv));
    ASSERT(recv == 33);
    ASSERT(!cbPop(&buffer, &recv));
}

static void test_cb_overwrite_if_full(void)
{
    cbuffer buffer;
    ASSERT(cbInit(&buffer, storage_u8, CAP_3, sizeof(uint8_t), OVERWRITE_IF_FULL));
 
    uint8_t val1 = 11, val2 = 22, val3 = 33, val4 = 44, recv = 0;
 
    ASSERT(cbPush(&buffer, &val1));
    ASSERT(cbPush(&buffer, &val2));
 
    ASSERT(cbPush(&buffer, &val3)); /* overwrites 11 */
    ASSERT(cbDropped(&buffer) == 1);
    ASSERT(cbPush(&buffer, &val4)); /* overwrites 22 */
    ASSERT(cbDropped(&buffer) == 2);
    ASSERT(cbCount(&buffer) == 2);
 
    ASSERT(cbPeekAt(&buffer, &recv, 0));
    ASSERT(recv == 33);
    ASSERT(cbPeekAt(&buffer, &recv, 1));
    ASSERT(recv == 44);
 
    ASSERT(cbPop(&buffer, &recv));
    ASSERT(recv == 33);
    ASSERT(cbPop(&buffer, &recv));
    ASSERT(recv == 44);
    ASSERT(!cbPop(&buffer, &recv));
}

static void test_cb_peek_at(void)
{
    cbuffer buffer;
    ASSERT(cbInit(&buffer, storage_u8, CAP_4, sizeof(uint8_t), IGNORE_IF_FULL));
 
    uint8_t val1 = 5, val2 = 10, recv = 0;
 
    ASSERT(!cbPeekAt(&buffer, &recv, 0)); /* empty */
 
    ASSERT(cbPush(&buffer, &val1));
    ASSERT(cbPush(&buffer, &val2));
 
    ASSERT(cbPeekAt(&buffer, &recv, 0));
    ASSERT(recv == 5);
    ASSERT(cbPeekAt(&buffer, &recv, 1));
    ASSERT(recv == 10);
    ASSERT(!cbPeekAt(&buffer, &recv, 2));
 
    /* Peeking does not remove anything */
    ASSERT(cbCount(&buffer) == 2);
}

static void test_cb_peek_at_after_wraparound(void)
{
    cbuffer buffer;
    ASSERT(cbInit(&buffer, storage_u8, CAP_4, sizeof(uint8_t), IGNORE_IF_FULL));
 
    uint8_t v = 0, recv = 0;
 
    for (v = 1; v <= 3; v++)
    {
        ASSERT(cbPush(&buffer, &v));
    }
    ASSERT(cbPop(&buffer, &recv));
    ASSERT(cbPop(&buffer, &recv)); /* tail is now at slot 2 */
 
    v = 4;
    ASSERT(cbPush(&buffer, &v));
    v = 5;
    ASSERT(cbPush(&buffer, &v)); /* head wrapped around to slot 1 */
 
    ASSERT(cbCount(&buffer) == 3);
    ASSERT(cbPeekAt(&buffer, &recv, 0));
    ASSERT(recv == 3);
    ASSERT(cbPeekAt(&buffer, &recv, 1));
    ASSERT(recv == 4);
    ASSERT(cbPeekAt(&buffer, &recv, 2));
    ASSERT(recv == 5);
    ASSERT(!cbPeekAt(&buffer, &recv, 3));
}

static void test_cb_wraparound(void)
{
    cbuffer buffer;
    ASSERT(cbInit(&buffer, storage_u8, CAP_4, sizeof(uint8_t), IGNORE_IF_FULL));
 
    uint8_t next = 0;
    uint8_t expected = 0;
 
    /* Bursts of 3 in a buffer of 4 slots shift the start position every round */
    for (uint8_t round = 0; round < 10; round++)
    {
        for (uint8_t k = 0; k < 3; k++)
        {
            ASSERT(cbPush(&buffer, &next));
            next++;
        }
        ASSERT(cbCount(&buffer) == 3);
        ASSERT(cbSpace(&buffer) == 0);
 
        for (uint8_t k = 0; k < 3; k++)
        {
            uint8_t recv = 0xFF;
            ASSERT(cbPop(&buffer, &recv));
            ASSERT(recv == expected);
            expected++;
        }
        ASSERT(cbCount(&buffer) == 0);
    }
 
    ASSERT(cbDropped(&buffer) == 0);
}

static void test_cb_dropped_saturates(void)
{
    cbuffer buffer;
    ASSERT(cbInit(&buffer, storage_u8, CAP_2, sizeof(uint8_t), IGNORE_IF_FULL));
 
    uint8_t val = 1;
    ASSERT(cbPush(&buffer, &val)); /* the only usable slot */
 
    for (uint16_t i = 0; i < 300; i++)
    {
        ASSERT(!cbPush(&buffer, &val));
    }
 
    ASSERT(cbDropped(&buffer) == 255);
}

static void test_cb_reset(void)
{
    cbuffer buffer;
    ASSERT(cbInit(&buffer, storage_u8, CAP_3, sizeof(uint8_t), IGNORE_IF_FULL));
 
    uint8_t val = 99, recv = 0;
    ASSERT(cbPush(&buffer, &val));
    ASSERT(cbPush(&buffer, &val));
    ASSERT(!cbPush(&buffer, &val)); /* creates a dropped element */
    ASSERT(cbDropped(&buffer) == 1);
 
    cbReset(&buffer);
 
    ASSERT(buffer.head == 0);
    ASSERT(buffer.tail == 0);
    ASSERT(buffer.dropped == 0);
    ASSERT(cbCount(&buffer) == 0);
 
    /* Usable again after reset */
    ASSERT(cbPush(&buffer, &val));
    ASSERT(cbPop(&buffer, &recv));
    ASSERT(recv == 99);
}
static void test_cb_null_arguments(void)
{
    cbuffer buffer;
    ASSERT(cbInit(&buffer, storage_u8, CAP_4, sizeof(uint8_t), IGNORE_IF_FULL));
 
    uint8_t v = 1;
    ASSERT(cbPush(&buffer, &v));
 
    ASSERT(!cbPush(NULL, &v));
    ASSERT(!cbPush(&buffer, NULL));
    ASSERT(!cbPop(NULL, &v));
    ASSERT(!cbPop(&buffer, NULL));
    ASSERT(!cbPeekAt(NULL, &v, 0));
    ASSERT(!cbPeekAt(&buffer, NULL, 0));
    ASSERT(cbCount(NULL) == 0);
    ASSERT(cbSpace(NULL) == 0);
    ASSERT(cbDropped(NULL) == 0);
    cbReset(NULL); /* must not crash */
 
    /* The failed calls did not change anything */
    ASSERT(cbCount(&buffer) == 1);
}

static void test_cb_struct_elements_and_bounds(void)
{
    enum
    {
        CAP = 5,
        GUARD = 8,
        GUARD_BYTE = 0xA5
    };
 
    /* Guard bytes before and after the storage detect writes out of bounds */
    uint8_t raw[GUARD + CAP * sizeof(reading) + GUARD];
    memset(raw, GUARD_BYTE, sizeof raw);
 
    cbuffer buffer;
    ASSERT(cbInit(&buffer, raw + GUARD, CAP, sizeof(reading), OVERWRITE_IF_FULL));
 
    for (uint16_t i = 0; i < 40; i++)
    {
        reading in = {(int16_t)(i - 20), (uint16_t)(i * 3), 1000UL + i};
        ASSERT(cbPush(&buffer, &in));
    }
 
    /* 5 slots hold 4 elements: the last four pushed (i = 36..39) are left */
    ASSERT(cbCount(&buffer) == 4);
    ASSERT(cbDropped(&buffer) == 36);
 
    for (uint8_t k = 0; k < 4; k++)
    {
        uint16_t i = 36U + k;
        reading out;
        ASSERT(cbPeekAt(&buffer, &out, k));
        ASSERT(out.temperature == (int16_t)(i - 20));
        ASSERT(out.light == (uint16_t)(i * 3));
        ASSERT(out.timestamp == 1000UL + i);
    }
 
    for (uint8_t k = 0; k < 4; k++)
    {
        uint16_t i = 36U + k;
        reading out;
        ASSERT(cbPop(&buffer, &out));
        ASSERT(out.temperature == (int16_t)(i - 20));
        ASSERT(out.light == (uint16_t)(i * 3));
        ASSERT(out.timestamp == 1000UL + i);
    }
    reading none;
    ASSERT(!cbPop(&buffer, &none));
 
    for (uint8_t g = 0; g < GUARD; g++)
    {
        ASSERT(raw[g] == GUARD_BYTE);
        ASSERT(raw[sizeof raw - 1U - g] == GUARD_BYTE);
    }
}

int main(void)
{
    printf("=== STARTING ALL CIRCULAR BUFFER TESTS ===\n");
 
    RUN_TEST(test_cb_init);
    RUN_TEST(test_cb_init_rejects_bad_arguments);
    RUN_TEST(test_cb_push_pop_basic);
    RUN_TEST(test_cb_counts_and_spaces);
    RUN_TEST(test_cb_ignore_if_full);
    RUN_TEST(test_cb_overwrite_if_full);
    RUN_TEST(test_cb_peek_at);
    RUN_TEST(test_cb_peek_at_after_wraparound);
    RUN_TEST(test_cb_wraparound);
    RUN_TEST(test_cb_dropped_saturates);
    RUN_TEST(test_cb_reset);
    RUN_TEST(test_cb_null_arguments);
    RUN_TEST(test_cb_struct_elements_and_bounds);
 
    printf("=== ALL TESTS PASSED SUCCESSFULLY ===\n");
    return 0;
}