/**
 *
 * @file usci.c
 * @author Michal Potrzebicz
 * @date 2013-03-20
 *
 * @brief Common USCI module code for MSP430 MCU's
 *
 * @section LICENSE
 *
 * Copyright (c) 2013, Michal Potrzebicz <michal@elevendroids.com>
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

#ifdef __MSP430_HAS_USCI__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "platform/msp430.h"
#include "usci.h"
#include "gpio_pin.h"

static const UsciModule usci_modules[USCI_MODULE_COUNT] = {
	// USCI A0
	{
		.ctl0 = &UCA0CTL0,
		.ctl1 = &UCA0CTL1,
		.br0 = &UCA0BR0,
		.br1 = &UCA0BR1,
		.mctl = &UCA0MCTL,
		.stat = &UCA0STAT,
		.rxbuf = &UCA0RXBUF,
		.txbuf = &UCA0TXBUF,
		.abctl = &UCA0ABCTL,
		.irtctl = &UCA0IRTCTL,
		.irrctl = &UCA0IRRCTL,
		.ie = &IE2,
		.ifg = &IFG2,
		.flags = {
			.rxifg = UCA0RXIFG,
			.txifg = UCA0TXIFG,
			.rxie = UCA0RXIE,
			.txie = UCA0TXIE
		},
		.pins = {
			.rxd = UCA0RXD,
			.txd = UCA0TXD,
			.somi = UCA0SOMI,
			.simo = UCA0SIMO,
			.ste = UCA0STE,
			.clk = UCA0CLK
		}
	},
	// USCI B0
	{
		.ctl0 = &UCB0CTL0,
		.ctl1 = &UCB0CTL1,
		.br0 = &UCB0BR0,
		.br1 = &UCB0BR1,
		.i2cie = &UCB0I2CIE,
		.stat = &UCB0STAT,
		.rxbuf = &UCB0RXBUF,
		.txbuf = &UCB0TXBUF,
		.i2coa = &UCB0I2COA,
		.i2csa = &UCB0I2CSA,
		.ie = &IE2,
		.ifg = &IFG2,
		.flags = {
			.rxifg = UCB0RXIFG,
			.txifg = UCB0TXIFG,
			.rxie = UCB0RXIE,
			.txie = UCB0TXIE
		},
		.pins = {
			.scl = UCB0SCL,
			.sda = UCB0SDA,
			.somi = UCB0SOMI,
			.simo = UCB0SIMO,
			.ste = UCB0STE,
			.clk = UCB0CLK
		}
	}
	//TODO: USCI A1, B1, etc.
};

const __attribute__((noinline)) UsciModule *Usci_GetModule(int usci)
{
	//TODO: check if usci is valid
	return &usci_modules[usci];
}

#endif /* __MSP430_HAS_USCI__ */

