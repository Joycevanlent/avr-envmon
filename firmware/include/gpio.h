/**
 * @file gpio.h
 * @brief AVR GPIO interface.
 *
 * Provides GPIO pin definitions and functions for configuring,
 * reading and writing AVR microcontroller GPIO pins.
 */

#ifndef GPIO_H
#define GPIO_H

#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * @brief GPIO pin direction.
 */
typedef enum
{
    GPIO_INPUT,
    GPIO_OUTPUT
} gpioMode;

/**
 * @brief Represents an AVR GPIO pin.
 *
 * Contains pointers to the corresponding DDR, PORT and PIN
 * registers, together with a bit mask identifying the pin.
 */
typedef struct
{
    volatile uint8_t *ddr;
    volatile uint8_t *port;
    volatile uint8_t *pin;
    uint8_t mask;
} gpioPin;

/**
 * @brief Create a GPIO pin definition.
 *
 * @param letter AVR port letter (B, C or D).
 * @param num    Bit number within the port (0-7).
 *
 * The two-level macro expansion ensures that macro arguments
 * are expanded before token concatenation.
 */
#define GPIO_PIN_IMPL(letter, num) \
    {                              \
        &DDR##letter,              \
        &PORT##letter,             \
        &PIN##letter,              \
        (uint8_t)_BV(num)}

#define GPIO_PIN(letter, num) GPIO_PIN_IMPL(letter, num)

/**
 * @brief Configure a GPIO pin as input or output.
 *
 * @param p    Pointer to the GPIO pin definition.
 * @param mode Desired pin direction.
 */
void gpioSetMode(const gpioPin *p, gpioMode mode);

/**
 * @brief Set the output level of a GPIO pin.
 *
 * @param p    Pointer to the GPIO pin definition.
 * @param high true for HIGH, false for LOW.
 */
void gpioWrite(const gpioPin *p, bool high);

/**
 * @brief Read the current logic level of a GPIO pin.
 *
 * @param p Pointer to the GPIO pin definition.
 * @return true if HIGH, false if LOW.
 */
bool gpioRead(const gpioPin *p);

#endif // GPIO_H
