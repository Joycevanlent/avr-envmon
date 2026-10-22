/**
 * @file adc.h
 * @brief Provides an interface for the AVR analog-to-digital converter (ADC).
 *
 * Provides functions to initialize the ADC and read analog input channels.
 * The ADC uses AVCC as its voltage reference and is configured with a
 * prescaler of 128.
 */

#ifndef ADC_H
#define ADC_H

#include <stdint.h>

/**
 * @brief Initializes the ADC peripheral.
 *
 * Configures AVCC as the voltage reference, selects right-adjusted
 * conversion results, enables the ADC, and performs an initial
 * conversion to stabilize the readings.
 *
 * @param[in] channel ADC input channel to use for the initial conversion.
 *                    Valid channels on the current target are 0 to 5.
 */
void adcInit(uint8_t channel);

/**
 * @brief Reads an analog input channel.
 *
 * Selects the specified ADC channel, starts a conversion, waits for
 * completion, and returns the resulting ADC value.
 *
 * @param[in] channel ADC input channel to read. Valid channels on the
 *                    current target are 0 to 5.
 *
 * @return The ADC conversion result (0 to 1023 for a 10-bit ADC).
 *         Returns 0 if the channel is invalid.
 */
uint16_t adcRead(uint8_t channel);

#endif // ADC_H