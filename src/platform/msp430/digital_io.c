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
#include "digital_io.h"

/** 
 * @brief Container for port registers 
 */
typedef struct {
	volatile uint8_t *out;		///< Output register
	volatile const uint8_t *in;	///< Input register
	volatile uint8_t *dir;		///< Direction register
	volatile uint8_t *ren;		///< Pullup/pulldown resistor enable register
	volatile uint8_t *sel1;		///< Primary function select register
	volatile uint8_t *sel2;		///< Secondary function select register (some MCU's only)
} Msp430Port;

/**
 * @brief Pin structure - contains link to pin's port and a pin bit mask
 */
typedef struct {
	const Msp430Port *port; ///< Pointer to pin's port
	uint8_t mask;		///< Bit mask used to set/reset pin
} Msp430Pin;

/**
 * @brief Array of available IO ports.
 */
static const Msp430Port ports[] = {
	{ &P1OUT, &P1IN, &P1DIR, &P1REN, &P1SEL,
		#ifdef P1SEL2_
		&P1SEL2 
		#else
		0
		#endif
	},

	{ &P2OUT, &P2IN, &P2DIR, &P2REN, &P2SEL, 
		#ifdef P2SEL2_
		&P2SEL2 
		#else
		0
		#endif
	}

	#ifdef __MSP430_HAS_PORT3_R__
	,{ &P3OUT, &P3IN, &P3DIR, &P3REN, &P3SEL, 
		#ifdef P3SEL2_
		&P3SEL2 
		#else
		0
		#endif
	}
	#endif /* __MSP430_HAS_PORT3_R__ */
};

/**
 * @brief Array of all available IO pins
 */
static const Msp430Pin pins[] = {
	{ &ports[0], BIT0 }, // P1.0 0
	{ &ports[0], BIT1 }, // P1.1
	{ &ports[0], BIT2 }, // P1.2
	{ &ports[0], BIT3 }, // P1.3
	{ &ports[0], BIT4 }, // P1.4
	{ &ports[0], BIT5 }, // P1.5
	{ &ports[0], BIT6 }, // P1.6
	{ &ports[0], BIT7 }, // P1.7 7

	{ &ports[1], BIT0 }, // P2.0 8
	{ &ports[1], BIT1 }, // P2.1
	{ &ports[1], BIT2 }, // P2.2
	{ &ports[1], BIT3 }, // P2.3
	{ &ports[1], BIT4 }, // P2.4
	{ &ports[1], BIT5 }, // P2.5
	{ &ports[1], BIT6 }, // P2.6
	{ &ports[1], BIT7 }  // P2.7 15
	
	#ifdef __MSP430_HAS_PORT3_R__
	,
	{ &ports[2], BIT0 }, // P3.0 16
	{ &ports[2], BIT1 }, // P3.1
	{ &ports[2], BIT2 }, // P3.2
	{ &ports[2], BIT3 }, // P3.3
	{ &ports[2], BIT4 }, // P3.4
	{ &ports[2], BIT5 }, // P3.5
	{ &ports[2], BIT6 }, // P3.6
	{ &ports[2], BIT7 }  // P3.7 23
	#endif /* __MSP430_HAS_PORT3_R__ */
};


void Pin_Set(int pin)
{
	const Msp430Pin *port_pin = &pins[pin];
	*port_pin->port->out |= port_pin->mask;
}

int Pin_Get(int pin)
{
	const Msp430Pin *port_pin = &pins[pin];
	if (*port_pin->port->in & port_pin->mask) {
		return PIN_STATE_HIGH;
	}
	else {
		return PIN_STATE_LOW;
	}
}

void Pin_Reset(int pin)
{
	const Msp430Pin *port_pin = &pins[pin];
	*port_pin->port->out &= ~port_pin->mask;
}


void Pin_Toggle(int pin)
{
	const Msp430Pin *port_pin = &pins[pin];
	*port_pin->port->out ^= port_pin->mask;
}

void Pin_SetMode(int pin, int mode)
{
	const Msp430Pin *port_pin = &pins[pin];
	const Msp430Port *port = port_pin->port;

	*port->sel1 &= ~port_pin->mask;
	if (port->sel2) {
		*port->sel2 &= ~port_pin->mask;
	}

	if (mode & PIN_MODE_OUTPUT) {
		*port->ren &= ~port_pin->mask;
		*port->out &= ~port_pin->mask;
		*port->dir |= port_pin->mask;
	}
	else {
		*port->dir &= ~port_pin->mask;
		if (mode & (PIN_MODE_PULLUP | PIN_MODE_PULLDOWN)) {
			*port->ren |= port_pin->mask;

			if (mode & PIN_MODE_PULLUP) {
				*port->out |= port_pin->mask;
			}
			else {
				*port->out &= ~port_pin->mask;
			}
		}
		else {
			*port->ren &= ~port_pin->mask;
		}
	}
}

