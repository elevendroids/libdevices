/*
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
#include "platform/msp430/usci.h"

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
			.sda = UCB0SDA
		}
	}
	//TODO: USCI A1, B1, etc.
};

static UsciData usci_data[USCI_MODULE_COUNT];

const UsciModule *Usci_GetModule(int usci)
{
	return &usci_modules[usci];
}

void Usci_SetHandlers(int usci, UsciHandler tx_handler, UsciHandler rx_handler)
{
	volatile UsciData *data = &usci_data[usci];
	data->tx_handler = tx_handler;
	data->rx_handler = rx_handler;
}

void Usci_SetData(int usci, void *data)
{
	volatile UsciData *usci_handler_data = &usci_data[usci];
	usci_handler_data->data = data;	
}

#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCIAB0_Transmit_ISR(void)
{
	int usci_index;
	UsciData *data = NULL;
	const UsciModule *usci;
	
	if ((IE2 & UCA0TXIE) && (IFG2 & UCA0TXIFG))
		usci_index = USCI_A0;
	else if (((IE2 & UCB0TXIE) && (IFG2 & UCB0TXIFG)) 
				|| ((IE2 & UCB0RXIE) && (IFG2 & UCB0RXIFG) && (UCB0CTL0 & UCMODE_3)))
		usci_index = USCI_B0;
	else
		return;

	data = &usci_data[usci_index];
	if (data && data->tx_handler) {
		usci = Usci_GetModule(usci_index);
		if (data->tx_handler(usci, data->data))
			LPM3_EXIT;
	}
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCIAB0_Receive_ISR(void)
{
	int usci_index;
	UsciData *data = NULL;
	const UsciModule *usci;

	if ((IE2 & UCA0RXIE) && (IFG2 & UCA0RXIFG))
		usci_index = USCI_A0;
	else if (((IE2 & UCB0RXIE) && (IFG2 & UCB0RXIFG)) || (UCB0CTL0 & UCMODE_3))
		usci_index = USCI_B0;
	else
		return;

	data = &usci_data[usci_index];
	if (data && data->rx_handler) {
		usci = Usci_GetModule(usci_index);
		if (data->rx_handler(usci, data->data))
			LPM3_EXIT;
	}
}

#endif /* __MSP430_HAS_USCI__ */

