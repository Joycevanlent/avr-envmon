/**
 * @file protocol.h
 * @brief Defines the interface for formatting environmental monitor messages.
 *
 * Provides functions to format sensor measurements and sensor error
 * messages according to protocol version 1 (V1).
 *
 * Messages are formatted as text and terminated with CRLF (\r\n).
 * This module formats messages but does not transmit them.
 */

#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdbool.h>
#include <stddef.h>

#include "sensor_data.h"

/**
 * @brief Formats a sensor measurement message.
 *
 * Formats the supplied sensor data using the V1 message format:
 * "V1,DATA,TEMP=<temperature>,LIGHT=<brightness>\r\n"
 *
 * The temperature value is represented in tenths of a degree Celsius,
 * and brightness is the raw ADC reading.
 *
 * @param[out] buffer Destination buffer for the formatted message.
 * @param[in] bufferSize Size of the destination buffer in bytes,
 *                       including space for the null terminator.
 * @param[in] payload Pointer to the sensor data to format.
 *
 * @return true if the message was formatted successfully and fits
 *         in the buffer; false if an argument is invalid or the
 *         message does not fit.
 */
bool protocolFormatSensorPayload(
    char *buffer,
    size_t bufferSize,
    const sensorPayload *payload
);

/**
 * @brief Formats a sensor error message.
 *
 * Formats the V1 sensor error message:
 * "V1,ERR,SENSOR\r\n"
 *
 * @param[out] buffer Destination buffer for the formatted message.
 * @param[in] bufferSize Size of the destination buffer in bytes,
 *                       including space for the null terminator.
 *
 * @return true if the message was formatted successfully and fits
 *         in the buffer; false if the buffer is invalid or too small.
 */
bool protocolFormatSensorError(
    char *buffer,
    size_t bufferSize
);

#endif // PROTOCOL_H
