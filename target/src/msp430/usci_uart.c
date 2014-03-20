#include <msp430.h>

#ifdef __MSP430_HAS_USCI__

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>

#include "bus/serial.h"
#include "platform.h"
#include "gpio.h"
#include "usci.h"
#include "txprintf.h"

#if defined(USE_SERIAL_FIFO)
#include "fifo.h"
#endif

#if defined(USE_SERIAL_FIFO)
static bool usci_uart_tx_handler(const UsciModule *usci_, void *data)
{
	serial_t *serial = (serial_t *)data;
	const UsciModule *usci = serial->usci;
	uint8_t value;

	if (fifo_pop(&serial->tx_fifo, &value))
		*usci->txbuf = value;
	else
		*usci->ie &= ~usci->flags.txie;

	return false;
}

static bool usci_uart_rx_handler(const UsciModule *usci, void *data)
{
	serial_t *serial = (serial_t *)data;
	fifo_push(&serial->rx_fifo, *serial->usci->rxbuf);
	return false;
}
#endif

bool serial_open(serial_t *serial, int device, uint32_t baud, uint8_t char_size, serial_parity parity, uint8_t stop_bits)
{
	uint32_t divider;
	uint16_t fraction;
	bool oversampling;
	const UsciModule *usci;
	uint8_t config = 0x00;

	if (parity != PARITY_NONE)
	{
		config = UCPEN;
		if (parity == PARITY_EVEN)
			config |= UCPAR;
	}

	if (char_size == 7)
		config |= UC7BIT;
	if (stop_bits == 2)
		config |= UCSPB;

	divider = (F_CPU << 4) / baud;
	oversampling = (F_CPU / baud) >= 48;
	if (oversampling)
	{
		fraction = ((divider & 0xF8) & 0xF0);
		divider >>= 8;
	}
	else
	{
		fraction = ((divider & 0x0F) + 1) & 0x0E;
		divider >>= 4;
	}

	usci = Usci_GetModule(device);

	pin_init_function(usci->pins.rxd, MSP430_PIN_FUNCTION_1 | MSP430_PIN_FUNCTION_2);
	pin_init_function(usci->pins.txd, MSP430_PIN_FUNCTION_1 | MSP430_PIN_FUNCTION_2);

	*usci->ctl1 = UCSWRST | UCSSEL_2;
	*usci->ctl0 = config;
	*usci->br0 = divider & 0xFF;
	*usci->br1 = divider >> 8;
	*usci->mctl = (oversampling ? UCOS16 : 0x00) | fraction;

	*usci->ctl1 &= ~UCSWRST;
	
	serial->usci = usci;
#if defined(USE_SERIAL_FIFO)
	fifo_init(&serial->tx_fifo, serial->tx_buffer, SERIAL_TX_BUFFER_SIZE);
	fifo_init(&serial->rx_fifo, serial->rx_buffer, SERIAL_RX_BUFFER_SIZE);
	Usci_SetHandlers(device, &usci_uart_tx_handler, &usci_uart_rx_handler);
	Usci_SetData(device, serial);
	*usci->ie |= usci->flags.rxie;
	_EINT();	
#endif
	return true;
}

char serial_getc(serial_t *serial)
{
	char result;

#if defined(USE_SERIAL_FIFO)
	while (fifo_is_empty(&serial->rx_fifo));
	fifo_pop(&serial->rx_fifo, (uint8_t *)&result);
#else
	const UsciModule *usci = serial->usci;
	while ((*usci->ifg & usci->flags.rxifg) == 0);
	result = *usci->rxbuf;
#endif

	return result;
}

void serial_putc(serial_t *serial, char c)
{
	const UsciModule *usci = serial->usci;
#if defined(USE_SERIAL_FIFO)
	while (fifo_is_full(&serial->tx_fifo));
	fifo_push(&serial->tx_fifo, (uint8_t)c);
	*usci->ie |= usci->flags.txie;
#else
	while ((*usci->ifg & usci->flags.txifg) == 0);
	*usci->txbuf = c;
#endif
}

void serial_print(serial_t *serial, char *c)
{
	while (*c)
	{
		serial_putc(serial, *c++);
	}
}

void serial_println(serial_t *serial, char *c)
{
	serial_print(serial, c);
	serial_print(serial, "\r\n");
}

static void serial_printf_putc(putc_obj_t obj, char c)
{
	serial_putc((serial_t *)obj, c);
}

int serial_printf(serial_t *serial, char *format, ...)
{
	va_list args;
	int result;
	va_start(args, format);
	result = txprintf(&serial_printf_putc, serial, format, args);
	va_end(args);
	return result;
}

#endif
