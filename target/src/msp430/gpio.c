#include <msp430.h>
#include <stdbool.h>
#include <stddef.h>
#include "platform.h"
#include "gpio.h"
#include "gpio_map.h"
#include "gpio_pin.h"

const gpio_t * port_init(int port_id)
{
	return gpio_ports[port_id];
}

void port_write(gpio_t *port, uint8_t value)
{
	*port->out = value;
}

uint8_t port_read(gpio_t *port)
{
	return *port->in;
}

void pin_init(gpio_pin_t *pin, GpioPin pin_id)
{
	pin->port = gpio_ports[(pin_id >> 8) & 0xFF];
	pin->mask = pin_id & 0xFF;
}

void pin_set_direction(gpio_pin_t *pin, GpioDirection direction)
{
	*pin->port->out &= ~pin->mask;
	if (direction == DIR_INPUT)
		*pin->port->dir &= ~pin->mask;
	else
		*pin->port->dir |= pin->mask;
}

void pin_set(gpio_pin_t *pin, bool state)
{
	if (state)
		*pin->port->out |= pin->mask;
	else
		*pin->port->out &= ~pin->mask;
}

bool pin_get(gpio_pin_t *pin)
{
	return ((*pin->port->in & pin->mask) != 0);
}

void pin_toggle(gpio_pin_t *pin)
{
	*pin->port->out ^= pin->mask;
}

void pin_set_mode(gpio_pin_t *pin, GpioPinMode mode)
{
	if (mode == MODE_NORMAL)
		*pin->port->ren &= ~pin->mask;
	else
		*pin->port->ren |= ~pin->mask;
	pin_set(pin, (mode == MODE_PULLUP));
}

void pin_init_function(GpioPin pin_id, int function)
{
	gpio_pin_t pin;
	pin_init(&pin, pin_id);

	if (pin.port->sel != NULL)
	{
		if (function & MSP430_PIN_FUNCTION_1)
			*pin.port->sel |= pin.mask;
		else
			*pin.port->sel &= ~pin.mask;
	}

	if (pin.port->sel2 != NULL)
	{
		if (function & MSP430_PIN_FUNCTION_2)
			*pin.port->sel2 |= pin.mask;
		else
			*pin.port->sel2 &= ~pin.mask;
	}
}
