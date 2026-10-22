
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void run_cb_tests(void);
void run_crc_tests(void);

int main(int argc, char *argv[])
{
    if (argc == 1 || strcmp(argv[1], "all") == 0)
    {
        printf("=== STARTING ALL UNIT TESTS ===\n\n");

        run_cb_tests();
        run_crc_tests();

        printf("\n=== ALL UNIT TESTS PASSED ===\n");
        return EXIT_SUCCESS;
    }

    if (strcmp(argv[1], "cb") == 0)
    {
        printf("=== STARTING CIRCULAR BUFFER TESTS ===\n\n");
        run_cb_tests();
        printf("\n=== CIRCULAR BUFFER TESTS PASSED ===\n");
        return EXIT_SUCCESS;
    }

    if (strcmp(argv[1], "crc") == 0)
    {
        printf("=== STARTING CRC-8 TESTS ===\n\n");
        run_crc_tests();
        printf("\n=== CRC-8 TESTS PASSED ===\n");
        return EXIT_SUCCESS;
    }

    fprintf(stderr, "Usage: %s [all|cb|crc]\n", argv[0]);
    return EXIT_FAILURE;
}