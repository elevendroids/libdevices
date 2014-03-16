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
#include <stdbool.h>
#include "platform/msp430.h"
#include "platform/msp430/port.h"
#include "digital_io.h"

void Port_Write(unsigned int port, uint8_t value)
{
	const Msp430Port *port_ptr = Msp430_GetPort(port);
	*port_ptr->out = value;
}

uint8_t Port_Read(unsigned int port)
{
	const Msp430Port *port_ptr = Msp430_GetPort(port);
	return *port_ptr->in;
}

void Port_SetMode(unsigned int port, unsigned int mode)
{
	const Msp430Port *port_ptr = Msp430_GetPort(port);
	uint8_t ren = 0x00;
	uint8_t out = 0x00;
	uint8_t dir = 0x00;

	if ((mode & PORT_MODE_OUTPUT) != 0)
	{
		dir = 0xFF;
	}
	else if ((mode & (PORT_MODE_PULLUP | PORT_MODE_PULLDOWN)) != 0)
	{
		ren = 0xFF;
		if ((mode & PORT_MODE_PULLUP) != 0)
		{
			out = 0xFF;
		}
	}

	*port_ptr->ren = ren;
	*port_ptr->out = out;
	*port_ptr->dir = dir;
}

void Pin_Set(unsigned int pin, bool state)
{
	uint8_t mask;
	const Msp430Port *port = Msp430_GetPinPort(pin, &mask);

	if (state)
	{
		*port->out |= mask;
	}
	else
	{
		*port->out &= ~mask;
	}
}

bool Pin_Get(unsigned int pin)
{
	uint8_t mask;
	const Msp430Port *port = Msp430_GetPinPort(pin, &mask);
	return ((*port->in & mask) != 0);
}

void Pin_Toggle(unsigned int pin)
{
	uint8_t mask;
	const Msp430Port *port = Msp430_GetPinPort(pin, &mask);
	*port->out ^= mask;
}

void Pin_SetMode(unsigned int pin, unsigned int mode)
{
	uint8_t mask;
	const Msp430Port *port = Msp430_GetPinPort(pin, &mask);

	if (mode == PIN_MODE_INPUT)
	{
		*port->dir &= ~mask;
	}
	else
	{
		*port->dir |= mask;
		*port->out &= ~mask;
	}
}

void Pin_SetPulldown(unsigned int pin, bool pulldown)
{
	uint8_t mask;
	const Msp430Port *port = Msp430_GetPinPort(pin, &mask);
	
	if (pulldown) {
		*port->ren |= mask;
		*port->out &= ~mask;
	}
	else {
		*port->ren &= ~mask;
	}
}

void Pin_SetPullup(unsigned int pin, bool pullup)
{
	uint8_t mask;
	const Msp430Port *port = Msp430_GetPinPort(pin, &mask);
	
	if (pullup)
	{
		*port->ren |= mask;
		*port->out |= mask;
	}
	else
	{
		*port->ren &= ~mask;
		*port->out &= ~mask;
	}
}





