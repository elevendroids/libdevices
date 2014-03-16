#ifndef _FIFO_H
#define _FIFO_H

#include <stdint.h>
#include <stdbool.h>

typedef struct fifo_tag {
	volatile uint8_t head;
	volatile uint8_t tail;
	uint8_t *buffer;
	uint8_t size;
} fifo_t;

void fifo_init(fifo_t *fifo, uint8_t *buffer, uint8_t size);

bool fifo_push(fifo_t *fifo, uint8_t value);
bool fifo_pop(fifo_t *fifo, uint8_t *value);

bool fifo_is_empty(fifo_t *fifo);
bool fifo_is_full(fifo_t *fifo);

void fifo_flush(fifo_t *fifo);


#endif
