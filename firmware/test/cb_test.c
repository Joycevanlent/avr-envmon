#include "circularbuffer.h"

#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define RUN_TEST(test_fn) do { \
    printf("Running %s...", #test_fn); \
    test_fn(); \
    printf(" [PASS]\n"); \
} while (0)

#define CAP_4 4
static uint8_t storage_u8[CAP_4 * sizeof(uint8_t)];

#define CAP_3 3
static uint32_t storage_u32[CAP_3 * sizeof(uint32_t)];

void test_cb_init(void)
{
    cbuffer buf;
    bool res = cbInit(&buf, storage_u8, CAP_4, sizeof(uint8_t), IGNORE_IF_FULL);
    
    assert(res == true);
    assert(buf.data == storage_u8);
    assert(buf.capacity == CAP_4);
    assert(buf.elementSize == sizeof(uint8_t));
    assert(buf.head == 0);
    assert(buf.tail == 0);
    assert(buf.dropped == 0);
    
    assert(cbInit(NULL, storage_u8, CAP_4, sizeof(uint8_t), IGNORE_IF_FULL) == false);
    assert(cbInit(&buf, NULL, CAP_4, sizeof(uint8_t), IGNORE_IF_FULL) == false);
    assert(cbInit(&buf, storage_u8, 0, sizeof(uint8_t), IGNORE_IF_FULL) == false);
    assert(cbInit(&buf, storage_u8, CAP_4, 0, IGNORE_IF_FULL) == false);
}

void test_cb_push_pop_basic(void)
{
    cbuffer buf;
    cbInit(&buf, (uint8_t*)storage_u32, CAP_3, sizeof(uint32_t), IGNORE_IF_FULL);
    
    uint32_t send1 = 42;
    uint32_t send2 = 84;
    uint32_t recv = 0;
    
    assert(cbPop(&buf, &recv) == false);
    assert(cbPush(&buf, &send1) == true);
    assert(cbPush(&buf, &send2) == true);
    assert(cbPop(&buf, &recv) == true);
    assert(recv == 42);
    assert(cbPop(&buf, &recv) == true);
    assert(recv == 84);
    assert(cbPop(&buf, &recv) == false);
}

void test_cb_counts_and_spaces(void)
{
    cbuffer buf;
    cbInit(&buf, storage_u8, CAP_4, sizeof(uint8_t), IGNORE_IF_FULL);
    
    assert(cbCount(&buf) == 0);
    assert(cbSpace(&buf) == 3); 
    
    uint8_t val = 10;
    cbPush(&buf, &val);
    
    assert(cbCount(&buf) == 1);
    assert(cbSpace(&buf) == 2);
    
    cbPush(&buf, &val);
    cbPush(&buf, &val);
    
    assert(cbCount(&buf) == 3);
    assert(cbSpace(&buf) == 0);
}

void test_cb_ignore_if_full(void)
{
    cbuffer buf;
    cbInit(&buf, storage_u8, CAP_3, sizeof(uint8_t), IGNORE_IF_FULL);
    
    uint8_t val1 = 11, val2 = 22, val3 = 33, recv = 0;
    
    assert(cbPush(&buf, &val1) == true);
    assert(cbPush(&buf, &val2) == true);
    assert(cbPush(&buf, &val3) == false);
    assert(cbDropped(&buf) == 1);
    assert(cbPop(&buf, &recv) == true);
    assert(recv == 11);
}

void test_cb_overwrite_if_full(void)
{
    cbuffer buf;
    cbInit(&buf, storage_u8, CAP_3, sizeof(uint8_t), OVERWRITE_IF_FULL);
    
    uint8_t val1 = 11, val2 = 22, val3 = 33, recv = 0;
    
    cbPush(&buf, &val1);
    cbPush(&buf, &val2);
    
    assert(cbPush(&buf, &val3) == true);
    assert(cbDropped(&buf) == 1);
    assert(cbPop(&buf, &recv) == true);
    assert(recv == 22);
    assert(cbPop(&buf, &recv) == true);
    assert(recv == 33);
}

void test_cb_peek_at(void)
{
    cbuffer buf;
    cbInit(&buf, storage_u8, CAP_4, sizeof(uint8_t), IGNORE_IF_FULL);
    
    uint8_t val1 = 5, val2 = 10, recv = 0;
    
    cbPush(&buf, &val1);
    cbPush(&buf, &val2);
    
    assert(cbPeekAt(&buf, &recv, 0) == true);
    assert(recv == 5);
    assert(cbPeekAt(&buf, &recv, 1) == true);
    assert(recv == 10);
    assert(cbPeekAt(&buf, &recv, 2) == false);
}

void test_cb_reset(void)
{
    cbuffer buf;
    cbInit(&buf, storage_u8, CAP_4, sizeof(uint8_t), IGNORE_IF_FULL);
    
    uint8_t val = 99;
    cbPush(&buf, &val);
    cbPush(&buf, &val);
    
    cbReset(&buf);
    
    assert(buf.head == 0);
    assert(buf.tail == 0);
    assert(buf.dropped == 0);
    assert(cbCount(&buf) == 0);
}

int main(void)
{
    printf("=== STARTING ALL CIRCULAR BUFFER TESTS ===\n");
    
    RUN_TEST(test_cb_init);
    RUN_TEST(test_cb_push_pop_basic);
    RUN_TEST(test_cb_counts_and_spaces);
    RUN_TEST(test_cb_ignore_if_full);
    RUN_TEST(test_cb_overwrite_if_full);
    RUN_TEST(test_cb_peek_at);
    RUN_TEST(test_cb_reset);
    
    printf("=== ALL TESTS PASSED SUCCESSFULLY ===\n");
    return 0;
}
