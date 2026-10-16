#include "circularbuffer.h"

#include <stddef.h>
#include <string.h>

bool cbInit(cbuffer *buffer, uint8_t *dataStorage, uint8_t capacity, uint8_t elementSize, enum cbmode mode)
{
    if (buffer == NULL || dataStorage == NULL || capacity == 0 || elementSize == 0)
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

    uint8_t nextHead = buffer->head + 1;
    if (nextHead >= buffer->capacity)
    {
        nextHead = 0;
    }

    // Check if the buffer is full
    if (nextHead == buffer->tail)
    {
        buffer->dropped++;

        if (buffer->mode == IGNORE_IF_FULL)
        {
            return false;
        }
        else if (buffer->mode == OVERWRITE_IF_FULL)
        {
            buffer->tail++;
            if (buffer->tail >= buffer->capacity)
            {
                buffer->tail = 0;
            }
        }
    }

    memcpy(buffer->data + (buffer->head * buffer->elementSize), element, buffer->elementSize);

    buffer->head = nextHead;

    return true;
}

bool cbPop(cbuffer *buffer, void *element)
{
    if (buffer == NULL || element == NULL)
    {
        return false;
    }

    if (buffer->head == buffer->tail)
    {
        return false;
    }

    memcpy(element, buffer->data + (buffer->tail * buffer->elementSize), buffer->elementSize);

    buffer->tail++;
    if (buffer->tail >= buffer->capacity)
    {
        buffer->tail = 0;
    }

    return true;
}

bool cbPeekAt(const cbuffer *buffer, void *element, uint8_t index)
{
    if (buffer == NULL || element == NULL)
    {
        return false;
    }

    if (index >= cbCount(buffer))
    {
        return false;
    }

    uint8_t askedIndex = buffer->tail + index;
    if (askedIndex >= buffer->capacity)
    {
        askedIndex -= buffer->capacity;
    }

    memcpy(element, buffer->data + (askedIndex * buffer->elementSize), buffer->elementSize);

    return true;
}

uint8_t cbCount(const cbuffer *buffer)
{
    if (buffer == NULL)
    {
        return false;
    }

    uint8_t count = 0;

    if (buffer->head < buffer->tail)
    {
        count = (buffer->capacity - buffer->tail) + buffer->head;
    }
    else
    {
        count = buffer->head - buffer->tail;
    }

    return count;
}

uint8_t cbSpace(const cbuffer *buffer)
{
    if (buffer == NULL)
    {
        return false;
    }

    uint8_t space = (buffer->capacity - 1) - cbCount(buffer);

    return space;
}

uint8_t cbDropped(const cbuffer *buffer)
{
    if (buffer == NULL)
    {
        return false;
    }

    return buffer->dropped;
}

void cbReset(cbuffer *buffer)
{
    buffer->head = 0;
    buffer->tail = 0;
    buffer->dropped = 0;
}