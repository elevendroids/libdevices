#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <msp430.h>
#include "board.h"
#include "platform.h"
#include "gpio.h"

static volatile gpio_irq_handler_t gpio_irq_handler;

static bool is_valid_irq_pin(PinId pin_id)
{
	const PinId *irq_pin_id = gpio_irq_pins;
	while (*irq_pin_id != NC)
	{
		if (*irq_pin_id == pin_id)
			return true;
		irq_pin_id++;
	}
	return false;
}

void gpio_set_irq_handler(gpio_irq_handler_t handler)
{
	gpio_irq_handler = handler;
}

void pin_attach_interrupt(gpio_pin_t *pin, GpioIrqEdge mode)
{
	if (is_valid_irq_pin(pin->id))
	{
		_DINT();
		if (mode == EDGE_FALLING)
			*(pin->port->ies) |= pin->mask;
		else
			*(pin->port->ies) &= ~pin->mask;
		*(pin->port->ie) |= pin->mask;
		*(pin->port->ifg) &= ~pin->mask;
		_EINT();
	}
}

void pin_detach_interrupt(gpio_pin_t *pin)
{
	if (is_valid_irq_pin(pin->id))
	{
		_DINT();
		*(pin->port->ie) &= ~pin->mask;
		*(pin->port->ifg) &= ~pin->mask;
		_EINT();
	}
}

static bool PortIsrHandler(uint8_t port, uint8_t flags)
{
	const PinId *pin = gpio_irq_pins;
	PinId irq_pins;
	bool result = false;

	if (gpio_irq_handler)
	{
		irq_pins = Gpio_MakePin(port, flags);
		while ((*pin != NC) && flags)
		{
			if ((*pin & irq_pins) == *pin)
			{
				if (gpio_irq_handler(*pin))
					result = true;
				flags &= ~Gpio_PinMask(*pin);
			}
			pin++;
		}
	}
	return result;
}

#pragma vector=PORT1_VECTOR
__interrupt void Port1_ISR(void) 
{
	bool wakeup = PortIsrHandler(0, P1IFG);
	P1IFG = 0x00;
	if (wakeup)
		LPM3_EXIT;
}

#ifdef MSP430_HAS_PORT2
#pragma vector=PORT2_VECTOR
__interrupt void Port2_ISR(void)
{
	bool wakeup = PortIsrHandler(1, P2IFG);
	P2IFG = 0x00;
	if (wakeup)
		LPM3_EXIT;
}
#endif

