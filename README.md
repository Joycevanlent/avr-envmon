# avr-envmon

Bare-metal C firmware for an environment monitor on an **Arduino Uno (ATmega328P)**.
No Arduino libraries or framework: registers, interrupts and a small set of reusable modules.
 
> **Status: work in progress.**

## Goals
 
- Read a temperature sensor and a light sensor at a fixed interval
- Keep the last N measurements in a ring buffer and send them to a laptop over UART
- Event-driven design: interrupts only queue work, the main loop does it and sleeps when idle
- Later: dashboard on the laptop (.NET), HTTP/REST over Ethernet

## Hardware
 
| Part | Connection |
|---|---|
| Arduino Uno (ATmega328P, 16 MHz) | USB to laptop (serial at 9600 baud) |
| DS18B20 temperature sensor (1-Wire) | data on Arduino pin 4 (PD4), pull-up resistor to 5 V |
| LDR light sensor | voltage divider with a 10 kΩ resistor on A0 |
| Status LEDs | Arduino pins 13, 12 and 11 (each with a series resistor) |
 
<!-- TODO: add a photo or wiring diagram -->

## Building and flashing
 
Requirements: AVR 8-bit GNU toolchain (`avr-gcc`), `avrdude`, CMake and Ninja, all on your `PATH`.
 
```
cd firmware
mkdir build && cd build
cmake -G Ninja ..
cmake --build .
cmake --build . --target upload
```
 
The COM port used by the `upload` target is set in `CMakeLists.txt` (currently `COM3`).
Close any serial terminal before uploading. Serial settings: 9600 baud, 8 data bits, no parity.

## Design notes
 
- **No `malloc`.** All storage is static and sized at compile time (the Uno has 2 KB of RAM).
- **Ring buffer** (`circularbuffer.c`): generic over the element size, with storage provided by the caller.
  Safe between one producer and one consumer (for example main loop and ISR) without disabling interrupts,
  because each index (`head`, `tail`) is written by exactly one side. One slot stays empty to tell full from empty.
  The overwrite mode is only for use within a single context.
- **Short ISRs.** Interrupt routines only do the minimum (count a tick, move a byte) and the main loop does the real work.
- **Non-blocking.** No `_delay_ms` in the application: time is measured with the 1 ms tick.
- **Portable core.** Hardware-independent logic is kept apart from chip-specific drivers so it can move to another chip later.