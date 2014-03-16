#include <stdint.h>
#include <stdbool.h>
#include "fifo.h"

void fifo_init(fifo_t *fifo, uint8_t *buffer, uint8_t size)
{
	fifo->buffer = buffer;
	fifo->head = 0;
	fifo->tail = 0;
	fifo->size = size;
}

static uint8_t fifo_get_next(fifo_t *fifo, uint8_t value)
{
	uint8_t result;
	result = value + 1;
	if (result >= fifo->size)
		result = 0;
	return result;
}

bool fifo_push(fifo_t *fifo, uint8_t value)
{
	uint8_t new_head = fifo_get_next(fifo, fifo->head);

	if (new_head != fifo->tail)
	{
		fifo->buffer[fifo->head] = value;
		fifo->head = new_head;
		return true;
	}
	return false;
}

bool fifo_pop(fifo_t *fifo, uint8_t *value)
{
	if (fifo->head != fifo->tail)
	{
		*value = fifo->buffer[fifo->tail];
		fifo->tail = fifo_get_next(fifo, fifo->tail);
		return true;
	}
	return false;
}

bool fifo_is_empty(fifo_t *fifo)
{
	return (fifo->head == fifo->tail);
}

bool fifo_is_full(fifo_t *fifo)
{
	uint8_t next = fifo_get_next(fifo, fifo->head);
	return (next == fifo->tail);
}
