#include <stddef.h>
#include <stdint.h>
#include <msp430.h>
#include "platform/msp430.h"
#include "digital_io.h"
#include "platform/msp430/port.h"

#ifdef MSP430_PORT_INT_COUNT
static volatile PinIntCallback pin_int_callbacks[MSP430_PORT_INT_COUNT];
#endif

void Pin_AttachInterrupt(unsigned int pin, PinIntCallback callback, unsigned int mode)
{
	uint8_t mask;
	const Msp430Port *port = Msp430_GetPinPort(pin, &mask);
	if (port->ie) {
		_DINT();
		*port->ie |= mask;
		if (mode == PIN_INT_MODE_FALLING)
			*port->ies |= mask;
		else
			*port->ies &= ~mask;
		*port->ifg &= ~mask;
		pin_int_callbacks[pin] = callback;
		_EINT();
	}
}

void Pin_DetachInterrupt(unsigned int pin)
{
	uint8_t mask;
	const Msp430Port *port = Msp430_GetPinPort(pin, &mask);
	if (port->ie) {
		_DINT();
		pin_int_callbacks[pin] = NULL;
		*port->ie &= ~mask;
		*port->ifg &= ~mask;
		_EINT();
	}
}

static void PortIsrHandler(const unsigned int index, uint8_t flags)
{
	volatile PinIntCallback *callback = &pin_int_callbacks[index];
	while (flags) {
		if ((flags & 0x01) && (*callback))
			(*callback)();
		callback++;
		flags >>= 1;
	}
}

#pragma vector=PORT1_VECTOR
__interrupt void Port1_ISR(void) 
{
	PortIsrHandler(0, P1IFG);
	P1IFG = 0x00;
}

#ifdef MSP430_HAS_PORT2
#pragma vector=PORT2_VECTOR
__interrupt void Port2_ISR(void)
{
	PortIsrHandler(8, P2IFG);
	P2IFG = 0x00;
}
#endif

