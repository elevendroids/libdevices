/**
 *
 * @file digital_io.c
 * @author Michal Potrzebicz
 * @date 2014-01-05
 *
 * @brief Simple digital IO API implementation for MSP430 microcontroller family
 *
 * @section LICENSE
 *
 * Copyright (c) 2014, Michal Potrzebicz <michal@elevendroids.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following 
 * conditions are met:
 *
 *    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *      disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
 *      disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <msp430.h>
#include <stdint.h>
#include "platform/msp430.h"
#include "digital_io.h"

/** 
 * @brief Container for port registers 
 */
typedef struct {
	volatile uint8_t *out;		///< Output register
	volatile const uint8_t *in;	///< Input register
	volatile uint8_t *dir;		///< Direction register
	volatile uint8_t *ren;		///< Pullup/pulldown resistor enable register
	volatile uint8_t *ie;		///< Interrupt enable register
	volatile uint8_t *ies;		///< Interrupt mode register
	volatile uint8_t *ifg;		///< Interrupt flag register
	volatile uint8_t *sel;		///< Primary peripherial module function select register
	volatile uint8_t *sel2;		///< Secondary peripherial module function select register
} Msp430Port;

/**
 * @brief Array of available IO ports.
 */
static const Msp430Port ports[] = {
	{
		&P1OUT, 
		&P1IN,
		&P1DIR,
		&P1REN,
		&P1IE,
		&P1IES,
		&P1IFG,
		&P1SEL,
		&P1SEL2
	}
#ifdef MSP430_HAS_PORT2
	,{ 
		&P2OUT,
		&P2IN,
		&P2DIR,
		&P2REN,
		&P2IE,
		&P2IES,
		&P2IFG,
		&P2SEL,
		&P2SEL2
	} 
#endif
#ifdef MSP430_HAS_PORT3
	,{
		&P3OUT,
		&P3IN,
		&P3DIR,
		&P3REN, 
		.sel = &P3SEL,
		.sel2 = &P3SEL2
	}
#endif
};

static const Msp430Port *pin_port[] = {
	&ports[0], &ports[0], &ports[0], &ports[0], &ports[0], &ports[0], &ports[0], &ports[0]
	#ifdef MSP430_HAS_PORT2
	,&ports[1], &ports[1], &ports[1], &ports[1], &ports[1], &ports[1], &ports[1], &ports[1]
	#endif
	#ifdef MSP430_HAS_PORT3
	,&ports[2], &ports[2], &ports[2], &ports[2], &ports[2], &ports[2], &ports[2], &ports[2]
	#endif
};

static const uint8_t pin_mask[] = {
	BIT0, BIT1, BIT2, BIT3, BIT4, BIT5, BIT6, BIT7
	#ifdef MSP430_HAS_PORT2
	,BIT0, BIT1, BIT2, BIT3, BIT4, BIT5, BIT6, BIT7
	#endif
	#ifdef MSP430_HAS_PORT3
	,BIT0, BIT1, BIT2, BIT3, BIT4, BIT5, BIT6, BIT7
	#endif
};

#ifdef MSP430_PORT_INT_COUNT
static volatile PinIntCallback pin_int_callbacks[MSP430_PORT_INT_COUNT];
#endif

void Port_Write(int port, uint8_t value)
{
	const Msp430Port *port_ptr = &ports[port];
	*port_ptr->out = value;
}

uint8_t Port_Read(int port)
{
	const Msp430Port *port_ptr = &ports[port];
	return *port_ptr->in;
}

void Port_SetMode(int port, int mode)
{
	const Msp430Port *port_ptr = &ports[port];
	uint8_t ren = 0x00;
	uint8_t out = 0x00;
	uint8_t dir = 0x00;
	uint8_t sel = 0x00;
	uint8_t sel2 = 0x00;

	if (mode & PORT_MODE_OUTPUT) 
		dir = 0xFF;
	else if (mode & (PORT_MODE_PULLUP | PORT_MODE_PULLDOWN)) {
		ren = 0xFF;
		if (mode & PORT_MODE_PULLUP)
			out = 0xFF;
	}

	*port_ptr->ren = ren;
	*port_ptr->out = out;
	*port_ptr->dir = dir;

	if (port_ptr->sel) {
		if (mode & MSP430_PORT_MODE_SEL)
			sel = 0xFF;
		*port_ptr->sel = sel;
	}

	if (port_ptr->sel2) {
		if (mode & MSP430_PORT_MODE_SEL2)
			sel2 = 0xFF;
		*port_ptr->sel2 = sel2;
	}
}

void Pin_Set(int pin, uint8_t state)
{
	const Msp430Port *port = pin_port[pin];
	const uint8_t mask = pin_mask[pin];
	if (state == PIN_STATE_LOW)
		*port->out &= ~mask;
	else
		*port->out |= mask;
}

int Pin_Get(int pin)
{
	const Msp430Port *port = pin_port[pin];
	const uint8_t mask = pin_mask[pin];

	if (*port->in & mask)
		return PIN_STATE_HIGH;
	else
		return PIN_STATE_LOW;
}

void Pin_Toggle(int pin)
{
	const Msp430Port *port = pin_port[pin];
	const uint8_t mask = pin_mask[pin];
	*port->out ^= mask;
}

void Pin_SetMode(int pin, int mode)
{
	const Msp430Port *port = pin_port[pin];
	const uint8_t mask = pin_mask[pin];

	if (mode & PIN_MODE_OUTPUT) {
		*port->ren &= ~mask;
		*port->out &= ~mask;
		*port->dir |= mask;
	}
	else {
		*port->dir &= ~mask;
		if (mode & (PIN_MODE_PULLUP | PIN_MODE_PULLDOWN)) {
			*port->ren |= ~mask;
			if (mode & PIN_MODE_PULLUP)
				*port->out |= mask;
			else
				*port->out &= ~mask;
		}
		else {
			*port->ren &= ~mask;
			*port->out &= ~mask;
		}
	}

	if (port->sel) {
		if (mode & MSP430_PIN_MODE_SEL)
			*port->sel |= mask;
		else
			*port->sel &= ~mask;
	}

	if (port->sel2) {
		if (mode & MSP430_PIN_MODE_SEL2)
			*port->sel2 |= mask;
		else
			*port->sel2 &= ~mask;
	}
}

void Pin_AttachInterrupt(int pin, PinIntCallback callback, int mode)
{
	const Msp430Port *port = pin_port[pin];
	const uint8_t mask = pin_mask[pin];
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

void Pin_DetachInterrupt(int pin)
{
	const Msp430Port *port = pin_port[pin];
	const uint8_t mask = pin_mask[pin];
	if (port->ie) {
		_DINT();
		pin_int_callbacks[pin] = 0;
		*port->ie &= ~mask;
		*port->ifg &= ~mask;
		_EINT();
	}
}

static void PortIsrHandler(const int index, uint8_t flags)
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
	P1IFG = 0;
}

#ifdef MSP430_HAS_PORT2
#pragma vector=PORT2_VECTOR
__interrupt void Port2_ISR(void)
{
	PortIsrHandler(8, P2IFG);
	P2IFG = 0;
}
#endif

