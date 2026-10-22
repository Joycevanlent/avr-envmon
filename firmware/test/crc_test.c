
#include "crc8.h"

#include <stdio.h>
#include <stdlib.h>

#define ASSERT(expr) \
    do \
    { \
        if (!(expr)) \
        { \
            fprintf(stderr, "\n%s:%d: CHECK failed: %s\n", \
                    __FILE__, __LINE__, #expr); \
            exit(EXIT_FAILURE); \
        } \
    } while (0)

#define RUN_TEST(test_fn) \
    do \
    { \
        printf("Running %s...", #test_fn); \
        test_fn(); \
        printf(" [PASS]\n"); \
    } while (0)


static void test_crc8_empty_data(void)
{
    ASSERT(crc8Calculate(NULL, 0) == 0x00);
}

static void test_crc8_single_zero_byte(void)
{
    const uint8_t data[] = {0x00};

    ASSERT(crc8Calculate(data, sizeof data) == 0x00);
}

static void test_crc8_single_byte(void)
{
    const uint8_t data[] = {0x01};

    ASSERT(crc8Calculate(data, sizeof data) == 0x5E);
}

static void test_crc8_multiple_bytes(void)
{
    const uint8_t data[] = {0x02, 0x00};

    ASSERT(crc8Calculate(data, sizeof data) == 0x91);
}

static void test_crc8_dallas_maxim_reference_vector(void)
{
    const uint8_t data[] = {
        0x02, 0x1C, 0xB8, 0x01, 0x00, 0x00, 0x00
    };

    ASSERT(crc8Calculate(data, sizeof data) == 0xA2);
}

void run_crc_tests(void)
{
    RUN_TEST(test_crc8_empty_data);
    RUN_TEST(test_crc8_single_zero_byte);
    RUN_TEST(test_crc8_single_byte);
    RUN_TEST(test_crc8_multiple_bytes);
    RUN_TEST(test_crc8_dallas_maxim_reference_vector);
}