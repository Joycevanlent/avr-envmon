cmake_minimum_required(VERSION 3.20)

option(RUNNING_TESTS "Build unit tests for the Host PC instead or a microcontroller target" OFF)

if(RUNNING_TESTS)
    project(avr-sensor-station-tests C)

    add_executable(cb_test
        test/cb_test.c
        src/circularbuffer.c
    )

    target_include_directories(cb_test PRIVATE include)
    target_compile_options(cb_test PRIVATE -Wall -Wextra -g)

else()
    set(TARGET_PLATFORM "avr" CACHE STRING "Target microcontroller platform")

    if(TARGET_PLATFORM STREQUAL "avr")
        project(avr-sensor-station C)

        set(MCU atmega328p)
        set(F_CPU 16000000UL)
        set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/avr-toolchain.cmake)

        # --- HARDWARE CONFIGURATION VIA CMAKE ---
        set(SERIAL_BAUD "9600" CACHE STRING "Baudrate for serial communication")
        set(SERIAL_BUFFER_SIZE "32" CACHE STRING "Capacity of the serial send buffer")
        set(ADC_CHANNEL_LDR  "0" CACHE STRING "ADC channel for the brightness sensor (0-5)")

        add_executable(main
            src/main.c
            src/circularbuffer.c
            src/envmon_machine.c
            src/sensor_manager.c
            src/drivers/avr/serial.c
            src/drivers/avr/timer.c
            src/drivers/avr/power.c
            src/drivers/avr/adc.c
            src/drivers/avr/platform.c
        )

        target_include_directories(main PRIVATE include)
        target_compile_options(main PRIVATE -mmcu=${MCU} -DF_CPU=${F_CPU} -Os -Wall -Wextra)
        target_link_options(main PRIVATE -mmcu=${MCU})

        target_compile_definitions(main PRIVATE
            SEND_BUFFER_CAPACITY=${SERIAL_BUFFER_SIZE}
            BAUD=${SERIAL_BAUD}
            LDR_CHANNEL=${ADC_CHANNEL_LDR}
        )

        add_custom_command(TARGET main POST_BUILD
            COMMAND avr-objcopy -O ihex $<TARGET_FILE:main> ${CMAKE_BINARY_DIR}/main.hex
            COMMENT "Converting to hex"
        )

        add_custom_target(upload
            COMMAND avrdude -c arduino -p ${MCU} -P COM3 -b 115200 -U flash:w:${CMAKE_BINARY_DIR}/main.hex:i
            DEPENDS main
            COMMENT "Uploading to Arduino"
        )

    else()
        message(FATAL_ERROR "Unknown TARGET_PLATFORM: ${TARGET_PLATFORM}")
    endif()
endif()