/**
 *
 * @file port.c
 * @author Michal Potrzebicz
 * @date 2014-02-22
 *
 * @brief MSP430 digital port common code
 *
 * @section LICENSE
 *
 * Copyright (c) 2014, Michal Potrzebicz <michal@elevendroids.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the followin
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
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <msp430.h>
#include <stdint.h>
#include <stddef.h>
#include "platform/msp430.h"
#include "platform/msp430/port.h"

static const Msp430Port P1 = {
	&P1OUT,
	&P1IN,
	&P1DIR,
	&P1REN,
	&P1IE,
	&P1IES,
	&P1IFG,
	&P1SEL,
	&P1SEL2
};

#ifdef MSP430_HAS_PORT2
static const Msp430Port P2 = {
	&P2OUT,
	&P2IN,
	&P2DIR,
	&P2REN,
	&P2IE,
	&P2IES,
	&P2IFG,
	&P2SEL,
	&P2SEL2
};
#endif

#ifdef MSP430_HAS_PORT3
static const Msp430Port P3 = {
	&P3OUT,
	&P3IN,
	&P3DIR,
	&P3REN,
	.sel = &P3SEL,
	.sel2 = &P3SEL2
};
#endif

/**
 * @brief Array of available IO ports.
 */
static const Msp430Port *ports[] = { 
	 &P1
#ifdef MSP430_HAS_PORT2
	,&P2
#endif
#ifdef MSP430_HAS_PORT3
	,&P3
#endif
};

static const uint8_t pin_bit_mask[] = {
	BIT0, BIT1, BIT2, BIT3, BIT4, BIT5, BIT6, BIT7
};

static const Msp430Port *pin_ports[] = {
	 &P1, &P1, &P1, &P1, &P1, &P1, &P1, &P1
	,&P2, &P2, &P2, &P2, &P2, &P2, &P2, &P2
	,&P3, &P3, &P3, &P3, &P3, &P3, &P3, &P3
};

const Msp430Port* Msp430_GetPort(const unsigned int port)
{
	return ports[port];
}

const Msp430Port* Msp430_GetPinPort(const unsigned int pin, uint8_t *mask)
{
	*mask = pin_bit_mask[pin % 8];
	return pin_ports[pin];
}

void Msp430_SetPinFunction(unsigned int pin, unsigned int mode)
{
	uint8_t mask;
	const Msp430Port *port = Msp430_GetPinPort(pin, &mask);

	if (port->sel != NULL)
	{
		if ((mode & MSP430_PIN_FUNCTION_1) != 0)
		{
			*port->sel |= mask;
		}
		else
		{
			*port->sel &= ~mask;
		}
	}

	if (port->sel2 != NULL)
	{
		if ((mode & MSP430_PIN_FUNCTION_2) != 0)
		{
			*port->sel2 |= mask;
		}
		else
		{
			*port->sel2 &= ~mask;
		}
    }

}

