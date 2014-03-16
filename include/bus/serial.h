#ifndef SERIAL_H
#define SERIAL_H

#include <stdbool.h>
#include <stdint.h>
#include "serial_object.h"

typedef enum serial_parity_tag {
	PARITY_NONE,
	PARITY_ODD,
	PARITY_EVEN
} serial_parity;

bool serial_open(serial_t *serial, int device, uint32_t baud, uint8_t char_size, serial_parity parity, uint8_t stop_bits);
char serial_getc(serial_t *serial);
void serial_putc(serial_t *serial, char c);

void serial_read(serial_t *serial, uint8_t *buffer, int length);
void serial_write(serial_t *serial, uint8_t *buffer, int length);

void serial_print(serial_t *serial, char *c);
void serial_println(serial_t *serial, char *c);
int serial_printf(serial_t *serial, char *format, ...);

#endif /* SERIAL_H */
