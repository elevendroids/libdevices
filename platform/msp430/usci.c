#include <msp430.h>
#include <stdint.h>

static volatile uint8_t *USCIA_tx_data;
static volatile uint8_t USCIA_tx_count = 0;
static volatile uint8_t *USCIB_tx_data;
static volatile uint8_t USCIB_tx_count = 0; 

void USCIA_wait(void)
{
	while ((UCA0STAT & UCBUSY));
}

void USCIB_wait(void)
{
	while ((UCB0STAT & UCBUSY));
}

#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCI_Transmit(void)
{
	if (IFG2 & UCA0TXIFG) {
		if (USCIA_tx_count == 0) {
			IE2 &= ~UCA0TXIE;
		} else {
			USCIA_tx_count--;
			UCB0TXBUF = *USCIA_tx_data;
			USCIA_tx_data++;
		}
	} else if (IFG2 & UCB0TXIFG) {
		if (USCIB_tx_count == 0) {
			IE2 &= ~UCB0TXIE;
		} else {
			USCIB_tx_count--;
			UCB0TXBUF = *USCIB_tx_data;
			USCIB_tx_data++;
		}
	}

}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI_Receive(void)
{
	;
}
