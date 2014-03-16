#ifndef _SERIAL_OBJECT_H
#define _SERIAL_OBJECT_H

#include "usci.h"
#include "fifo.h"

#define SERIAL_TX_BUFFER_SIZE	16
#define SERIAL_RX_BUFFER_SIZE	16

typedef struct serial_tag {
	const UsciModule *usci;
#if defined(USE_SERIAL_FIFO)
	uint8_t tx_buffer[SERIAL_TX_BUFFER_SIZE];
	uint8_t rx_buffer[SERIAL_RX_BUFFER_SIZE];
	fifo_t tx_fifo;
	fifo_t rx_fifo;
#endif
} serial_t;

#endif
