
cmake_minimum_required(VERSION 3.20)

project(avr-envmon C)

# --- CONFIGURATION ---

set(BOARD "uno" CACHE STRING "Target board (e.g., uno or esp32)")
set(UPLOAD_PORT "COM3" CACHE STRING "Serial port for uploading")

option(BUILD_FIRMWARE "Build AVR firmware" ON)
option(BUILD_TESTS "Build host unit tests" ON)

# --- AVR FIRMWARE ---

if(BUILD_FIRMWARE)

    if(BOARD STREQUAL "uno")
        set(MCU atmega328p)
        set(F_CPU 16000000UL)

        set(PLATFORM_SOURCES
            src/drivers/avr/serial.c
            src/drivers/avr/timer.c
            src/drivers/avr/power.c
            src/drivers/avr/adc.c
            src/drivers/avr/onewire.c
            src/drivers/avr/platform.c
        )
    else()
        message(FATAL_ERROR "Unknown or unsupported board: ${BOARD}")
    endif()

    add_executable(main
        src/main.c
        src/circularbuffer.c
        src/envmon_machine.c
        src/sensor_manager.c
        src/ds18b20.c
        src/crc8.c
        ${PLATFORM_SOURCES}
    )

    target_include_directories(main PRIVATE
        include
        include/board/${BOARD}
    )

    target_compile_options(main PRIVATE
        -mmcu=${MCU}
        -DF_CPU=${F_CPU}
        -Os
        -Wall
        -Wextra
    )

    target_compile_definitions(main PRIVATE
        SEND_BUFFER_CAPACITY=32
        BAUD=9600
    )

    target_link_options(main PRIVATE
        -mmcu=${MCU}
    )

    add_custom_command(TARGET main POST_BUILD
        COMMAND avr-objcopy
            -O ihex
            $<TARGET_FILE:main>
            ${CMAKE_CURRENT_BINARY_DIR}/main.hex
        COMMENT "Converting firmware to HEX..."
    )

    add_custom_target(upload
        COMMAND avrdude
            -c arduino
            -p ${MCU}
            -P ${UPLOAD_PORT}
            -b 115200
            -U flash:w:${CMAKE_CURRENT_BINARY_DIR}/main.hex:i
        DEPENDS main
        COMMENT "Uploading to Arduino on port ${UPLOAD_PORT}..."
    )

endif()

# --- UNIT TESTS (HOST PC) ---

if(BUILD_TESTS)

    enable_testing()

    add_executable(test_runner
        test/main_test.c
        test/cb_test.c
        test/crc_test.c
        src/circularbuffer.c
        src/crc8.c
    )

    target_include_directories(test_runner PRIVATE
        include
    )

    target_compile_options(test_runner PRIVATE
        -Wall
        -Wextra
    )

    add_test(
        NAME CircularBufferTest
        COMMAND test_runner cb
    )

    add_test(
        NAME CRC8Test
        COMMAND test_runner crc
    )

endif()