#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Buffer modes: overwrite if full or ignore if full
 */
enum cbmode
{
    OVERWRITE_IF_FULL,
    IGNORE_IF_FULL
};
/**
 * @brief Structure representing the circular buffer
 */
typedef struct
{
    uint8_t *data;         // Pointer to the buffer data
    enum cbmode mode;      // Buffer mode
    uint8_t capacity;      // Maximum number of elements the buffer can hold
    volatile uint8_t head; // Index of the head of the buffer
    volatile uint8_t tail; // Index of the tail of the buffer
    uint8_t elementSize;   // Size of one element in the buffer
    uint8_t dropped;       // Number of elements thrown out of the buffer due to overflow
} cbuffer;
/** @brief Initialize the circular buffer
 *
 *  @param buffer Pointer to the circular buffer structure
 *  @param dataStorage Pointer to the data storage array
 *  @param capacity Maximum number of elements the buffer can hold
 *  @param elemSize Size of one element in the buffer
 *  @param mode Mode of the buffer (overwrite or ignore if full)
 *  @return true if the buffer was initialized successfully, false otherwise
 */
bool cbInit(cbuffer *buffer, uint8_t *dataStorage, uint8_t capacity, uint8_t elemSize, enum cbmode mode);
/**
 * @brief Push a value into the circular buffer
 * @param buffer Pointer to the circular buffer
 * @param value Value to be pushed into the buffer
 * @return true if the value was pushed successfully, false if the buffer is full and the value was not pushed
 */
bool cbPush(cbuffer *buffer, const void *element);
/**
 * @brief Pop a value from the circular buffer
 * @param buffer Pointer to the circular buffer
 * @param value Pointer to the variable where the popped value will be stored
 * @return true if a value was popped successfully, false if the buffer is empty
 */
bool cbPop(cbuffer *buffer, void *element);
/**
 * @brief Peek at a value in the circular buffer without removing it
 * @param buffer Pointer to the circular buffer
 * @param value Pointer to the variable where the peeked value will be stored
 * @param index Index of the value to peek at (0 is the oldest value)
 * @return true if the value was peeked successfully, false if the index is out of bounds
 */
bool cbPeekAt(const cbuffer *buffer, void *element, uint8_t index);
/**
 * @brief Amount of elements currently in the buffer
 * @param buffer Pointer to the circular buffer
 * @return Number of elements in the buffer
 */
uint8_t cbCount(const cbuffer *buffer);
/**
 * @brief Amount of free space in the buffer
 * @param buffer Pointer to the circular buffer
 * @return Number of free spaces in the buffer
 */
uint8_t cbSpace(const cbuffer *buffer);
/**
 * @brief Amount of elements that have been dropped due to overflow
 * @param buffer Pointer to the circular buffer
 * @return Number of dropped elements
 */
uint8_t cbDropped(const cbuffer *buffer);
/**
 * @brief Reset the circular buffer
 * @param buffer Pointer to the circular buffer
 */
void cbReset(cbuffer *buffer);

#endif // CIRCULARBUFFER_H