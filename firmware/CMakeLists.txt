cmake_minimum_required(VERSION 3.20)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/avr-toolchain.cmake)

project(avr-sensor-station C)

set(MCU atmega328p)
set(F_CPU 16000000UL)

add_executable(main
    src/main.c
    src/communication/usart.c
    src/utils/timer.c
)

target_include_directories(main PRIVATE include)

target_compile_options(main PRIVATE
    -mmcu=${MCU}
    -DF_CPU=${F_CPU}
    -Os
)

target_link_options(main PRIVATE
    -mmcu=${MCU}
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