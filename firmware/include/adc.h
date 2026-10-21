#ifndef ADC_H
#define ADC_H

#include <stdint.h>

/**
 * @brief Initializes the Analog-to-Digital Converter (ADC).
 *
 * Configures the ADC voltage reference (AVCC with an external capacitor at the AREF pin),
 * sets right-alignment for the data, configures the prescaler to 128 (resulting in a
 * 125kHz ADC clock speed given a 16MHz CPU frequency), and enables the ADC hardware.
 * Finally, performs an initial 'dummy' conversion to ensure subsequent readings are accurate.
 *
 * @param channel The initial analog channel (0 to 5) used for the dummy conversion.
 */
void adcInit(uint8_t channel);

/**
 * @brief Reads the analog value from a specific ADC channel.
 *
 * Switches the multiplexer to the specified channel, initiates the conversion,
 * and blocks until the measurement is fully completed.
 *
 * @param channel The analog channel to read (0 to 5, corresponding to A0 to A5).
 * @return uint16_t The 10-bit digital representation of the analog voltage (0 to 1023).
 *                  Returns 0 if an invalid channel is requested.
 */
uint16_t adcRead(uint8_t channel);

#endif // ADC_H