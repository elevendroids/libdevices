
#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "usci.h"

static volatile UsciData usci_data[USCI_MODULE_COUNT];

void Usci_SetHandlers(int usci, UsciHandler tx_handler, UsciHandler rx_handler)
{
	usci_data[usci].tx_handler = tx_handler;
	usci_data[usci].rx_handler = rx_handler;
}

void Usci_SetData(int usci, void *data)
{
	usci_data[usci].data = data;
}

static bool usci_handle_isr(int usci, bool tx_handler)
{
	UsciHandler handler;
	void *data = usci_data[usci].data;

	if (tx_handler)
		handler = usci_data[usci].tx_handler;
	else
		handler = usci_data[usci].rx_handler;

	return ((handler) && handler(NULL, data));
}

#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCIAB0_Transmit_ISR(void)
{
	bool wakeup = false;
	if ((IE2 & UCA0TXIE) && (IFG2 & UCA0TXIFG))
		wakeup = usci_handle_isr(USCI_A0, true);

	if (((IE2 & UCB0TXIE) && (IFG2 & UCB0TXIFG)) || ((UCB0CTL0 & UCMODE_3) && (IFG2 & UCB0RXIFG)))
		wakeup = usci_handle_isr(USCI_B0, true);

	if (wakeup)
		LPM3_EXIT;
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCIAB0_Receive_ISR(void)
{
	bool wakeup = false;

	if ((IE2 & UCA0RXIE) && (IFG2 & UCA0RXIFG))
		wakeup = usci_handle_isr(USCI_A0, false);
	
	if (((IE2 & UCB0RXIE) && (IFG2 & UCB0RXIFG)) || 
		((UCB0CTL0 & UCMODE_3) && (UCB0STAT & UCNACKIFG)))
		wakeup = usci_handle_isr(USCI_B0, false);

	if (wakeup)
		LPM3_EXIT;
}



