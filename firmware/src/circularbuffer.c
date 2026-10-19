#include "circularbuffer.h"

#include <stddef.h>
#include <string.h>

/*
 * Compiler barrier: stops the compiler from moving memory accesses across this
 * point. `volatile` only orders volatile accesses among themselves, so without
 * this the element copy could in theory be moved past the head/tail update.
 */
#if defined(__GNUC__)
#define CB_COMPILER_BARRIER() __asm__ __volatile__("" ::: "memory")
#else
#error "CB_COMPILER_BARRIER is not defined for this compiler"
#endif

/* Next index after `index`, wrapping at `capacity`. */
static uint8_t advance(uint8_t index, uint8_t capacity)
{
    index++;
    if (index >= capacity)
    {
        index = 0;
    }
    return index;
}

/*
 * Address of slot `index`.
 */
static uint8_t *slot(const cbuffer *buffer, uint8_t index)
{
    return buffer->data + ((uint16_t)index * buffer->elementSize);
}

/* Number of elements for one consistent pair of head/tail values. */
static uint8_t countFrom(uint8_t capacity, uint8_t head, uint8_t tail)
{
    uint8_t count = 0;
    if (head >= tail)
    {
        count = (uint8_t)(head - tail);
    }
    else
    {
        count = (uint8_t)(capacity - tail + head);
    }

    return count;
}

bool cbInit(cbuffer *buffer, uint8_t *dataStorage, uint8_t capacity, uint8_t elementSize, enum cbmode mode)
{
    if (buffer == NULL || dataStorage == NULL || elementSize == 0)
    {
        return false;
    }

    if (capacity < CB_MIN_CAPACITY || capacity > CB_MAX_CAPACITY)
    {
        return false;
    }

    if (mode != OVERWRITE_IF_FULL && mode != IGNORE_IF_FULL)
    {
        return false;
    }

    buffer->data = dataStorage;
    buffer->mode = mode;
    buffer->capacity = capacity;
    buffer->head = 0;
    buffer->tail = 0;
    buffer->elementSize = elementSize;
    buffer->dropped = 0;

    return true;
}

bool cbPush(cbuffer *buffer, const void *element)
{
    if (buffer == NULL || element == NULL)
    {
        return false;
    }

    uint8_t head = buffer->head;
    uint8_t nextHead = advance(head, buffer->capacity);
    uint8_t tail = buffer->tail;

    if (nextHead == tail)
    {
        if (buffer->dropped < UINT8_MAX)
        {
            buffer->dropped++;
        }

        if (buffer->mode == IGNORE_IF_FULL)
        {
            return false;
        }
        else
        {
            buffer->tail = advance(tail, buffer->capacity);
        }
    }

    memcpy(slot(buffer, head), element, buffer->elementSize);

    CB_COMPILER_BARRIER();
    buffer->head = nextHead;

    return true;
}

bool cbPop(cbuffer *buffer, void *element)
{
    if (buffer == NULL || element == NULL)
    {
        return false;
    }

    uint8_t tail = buffer->tail;

    if (buffer->head == tail)
    {
        return false;
    }

    CB_COMPILER_BARRIER();
    memcpy(element, slot(buffer, tail), buffer->elementSize);

    CB_COMPILER_BARRIER();
    buffer->tail = advance(tail, buffer->capacity);

    return true;
}

bool cbPeekAt(const cbuffer *buffer, void *element, uint8_t index)
{
    if (buffer == NULL || element == NULL)
    {
        return false;
    }

    uint8_t head = buffer->head;
    uint8_t tail = buffer->tail;

    if (index >= countFrom(buffer->capacity, head, tail))
    {
        return false;
    }

    CB_COMPILER_BARRIER();

    uint16_t position = (uint16_t)tail + index;
    if (position >= buffer->capacity)
    {
        position -= buffer->capacity;
    }

    memcpy(element, slot(buffer, (uint8_t)position), buffer->elementSize);

    return true;
}

uint8_t cbCount(const cbuffer *buffer)
{
    if (buffer == NULL)
    {
        return 0;
    }

    uint8_t head = buffer->head;
    uint8_t tail = buffer->tail;

    uint8_t count = countFrom(buffer->capacity, head, tail);

    return count;
}

uint8_t cbSpace(const cbuffer *buffer)
{
    if (buffer == NULL)
    {
        return 0;
    }

    uint8_t space = (uint8_t)((buffer->capacity - 1U) - cbCount(buffer));

    return space;
}

uint8_t cbDropped(const cbuffer *buffer)
{
    if (buffer == NULL)
    {
        return 0;
    }

    uint8_t dropped = buffer->dropped;

    return dropped;
}

void cbReset(cbuffer *buffer)
{
    if (buffer == NULL)
    {
        return;
    }

    buffer->head = 0;
    buffer->tail = 0;
    buffer->dropped = 0;
}