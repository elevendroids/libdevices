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
#include "platform/msp430/usci.h"

UsciData UsciXmitData[USCI_MODULE_COUNT];

void Usci_SetHandlers(int usci, UsciHandler tx_handler, UsciHandler rx_handler)
{
	volatile UsciData *data = &UsciXmitData[usci];
	data->tx_handler = tx_handler;
	data->rx_handler = rx_handler;
}

void Usci_SetData(int usci, void *data)
{
	volatile UsciData *usci_data = &UsciXmitData[usci];
	usci_data->data = data;	
}

#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCIAB0_Transmit_ISR(void)
{
	UsciData *data = NULL;
	
	if ((IE2 & UCA0TXIE) && (IFG2 & UCA0TXIFG))
		data = &UsciXmitData[USCI_A0];
	else if (((IE2 & UCB0TXIE) && (IFG2 & UCB0TXIFG)) 
				|| ((IE2 & UCB0RXIE) && (IFG2 & UCB0RXIFG) && (UCB0CTL0 & UCMODE_3)))
		data = &UsciXmitData[USCI_B0];

	if ((data && data->tx_handler) && (data->tx_handler(data->data)))
		LPM3_EXIT;
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCIAB0_Receive_ISR(void)
{
	UsciData *data = NULL;

	if ((IE2 & UCA0RXIE) && (IFG2 & UCA0RXIFG))
		data = &UsciXmitData[USCI_A0];
	else if (((IE2 & UCB0RXIE) && (IFG2 & UCB0RXIFG)) || (UCB0CTL0 & UCMODE_3))
		data = &UsciXmitData[USCI_B0];

	if ((data && data->rx_handler) && (data->rx_handler(data->data)))
		LPM3_EXIT;
}

#endif /* __MSP430_HAS_USCI__ */

