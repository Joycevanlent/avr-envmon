/**
 * @file circularbuffer.h
 * @brief Generic fixed-size circular (ring) buffer, safe to share between an
 * interrupt service routine and the main loop.
 * One slot is always kept empty to tell "full" from "empty", so the usable
 * capacity is `capacity - 1` elements.
 */

#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Smallest supported capacity. With one slot always kept empty, a
 * capacity of 1 would not be able to hold anything.
 */
#define CB_MIN_CAPACITY 2U

/**
 * @brief Largest supported capacity.
 */
#define CB_MAX_CAPACITY 128U

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
    uint8_t *data;         // Pointer to the storage owned by the caller
    enum cbmode mode;      // Buffer mode
    uint8_t capacity;      // Number of slots (usable elements: capacity - 1)
    volatile uint8_t head; // Next slot to write
    volatile uint8_t tail; // Next slot to read
    uint8_t elementSize;   // Size of one element in bytes
    uint8_t dropped;       // Elements lost to overflow (saturates at 255)
} cbuffer;

/**
 * @brief Initialize the circular buffer.
 *
 *
 * @param buffer      Pointer to the circular buffer structure
 * @param dataStorage Storage of at least capacity * elementSize bytes (stays owned by the caller)
 * @param capacity    Number of slots, CB_MIN_CAPACITY..CB_MAX_CAPACITY (usable: capacity - 1)
 * @param elementSize Size of one element in bytes
 * @param mode        OVERWRITE_IF_FULL or IGNORE_IF_FULL
 * @return true if the buffer was initialized, false if an argument is invalid
 */
bool cbInit(cbuffer *buffer, uint8_t *dataStorage, uint8_t capacity, uint8_t elementSize, enum cbmode mode);

/**
 * @brief Push one element into the buffer.
 *
 *
 * @param buffer  Pointer to the circular buffer
 * @param element Pointer to the element to copy in (elementSize bytes)
 * @return true if pushed. false if the buffer is full in IGNORE_IF_FULL mode
 *         (the element is dropped and counted) or an argument is NULL.
 *         In OVERWRITE_IF_FULL mode a full buffer drops the oldest element
 *         (counted) and returns true.
 */
bool cbPush(cbuffer *buffer, const void *element);

/**
 * @brief Remove the oldest element and copy it out.
 *
 *
 * @param buffer  Pointer to the circular buffer
 * @param element Pointer where the element is copied to (elementSize bytes)
 * @return true if an element was popped, false if the buffer is empty or an argument is NULL
 */
bool cbPop(cbuffer *buffer, void *element);

/**
 * @brief Copy an element out without removing it.
 *
 *
 * @param buffer  Pointer to the circular buffer
 * @param element Pointer where the element is copied to (elementSize bytes)
 * @param index   0 is the oldest element, cbCount() - 1 the newest
 * @return true if copied, false if the index is out of range or an argument is NULL
 */
bool cbPeekAt(const cbuffer *buffer, void *element, uint8_t index);

/**
 * @brief Number of elements currently in the buffer.
 *
 *
 * @param buffer Pointer to the circular buffer
 * @return Number of elements, 0 if buffer is NULL
 */
uint8_t cbCount(const cbuffer *buffer);

/**
 * @brief Number of elements that can still be pushed (capacity - 1 - count).
 *
 *
 * @param buffer Pointer to the circular buffer
 * @return Free slots, 0 if buffer is NULL
 */
uint8_t cbSpace(const cbuffer *buffer);

/**
 * @brief Number of elements lost to overflow since init or the last reset.
 *        The counter stays at 255 once it gets there.
 *
 * @param buffer Pointer to the circular buffer
 * @return Number of dropped elements, 0 if buffer is NULL
 */
uint8_t cbDropped(const cbuffer *buffer);

/**
 * @brief Empty the buffer and clear the dropped counter.
 *
 *
 * @param buffer Pointer to the circular buffer
 */
void cbReset(cbuffer *buffer);

#endif // CIRCULARBUFFER_H