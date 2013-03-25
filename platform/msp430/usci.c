#include <msp430.h>
#include <stdint.h>
#include "platform/msp430/usci.h"

#define USCI_MODE_I2C	BIT0
#define USCI_MODE_SPI	BIT1

volatile UsciTransaction *UsciB_transaction;
volatile UsciMessage *UsciB_message;
volatile uint8_t UsciB_message_counter;
volatile uint8_t *UsciB_data;
volatile uint8_t UsciB_data_counter;

void USCIB_I2cInit(uint8_t prescale)
{
	// Configure I2C pins
	P1SEL 	|= BIT6 | BIT7;
	P1SEL2 	|= BIT6 | BIT7;
	
	UCB0CTL1 = UCSSEL_2 + UCSWRST;
	UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC; 	// I2C master, synchronous
	UCB0BR0 = prescale;
	UCB0BR1 = 0x00;
	UCB0CTL1 &= ~UCSWRST;
	IE2 |= UCB0TXIE;
}

int UsciB_I2cTransaction(uint8_t address, UsciTransaction *transaction)
{
	// Set slave address
	UCB0I2CSA = address;
	//

	UsciB_transaction = transaction;
	UsciB_message = transaction->messages;
	UsciB_message_counter = transaction->message_count - 1;
	UsciB_data = UsciB_message->data;
	UsciB_data_counter = UsciB_message->length;

	if (UsciB_message->flags & USCI_MESSAGE_DIR_READ) {
		UCB0CTL1 &= ~UCTR;
		UCB0CTL1 |= UCTXSTT;
	} else {
		UCB0CTL1 |= UCTR + UCTXSTT;
	}
	while (UCB0STAT & UCBBUSY);
	return 0;
}

static inline void UsciB_SpiMasterHandler()
{
	_NOP();
}

static inline void UsciB_I2cTxHandler(void) 
{
	volatile UsciMessage *prev_message;
	int dir_changed;

	if (UsciB_data_counter == 0) {
		// fetch next message, if any
		if (UsciB_message_counter) {
			UsciB_message_counter--;
			prev_message = UsciB_message++;
			UsciB_data = UsciB_message->data;
			UsciB_data_counter = UsciB_message->length;
			dir_changed = (prev_message->flags & USCI_MESSAGE_DIR) != (UsciB_message->flags & USCI_MESSAGE_DIR);
			// if direction has changed, issue restart in read mode and exit
			if (dir_changed) {
				UCB0CTL1 &= ~UCTR;
				UCB0CTL1 |= UCTXSTT;
				IFG2 &= ~UCB0TXIFG;
			// if direction has not changed, issue restart unless message is flagged "NO_RESTART"
			} else if (!(UsciB_message->flags & USCI_I2C_NO_RESTART)) {
				UCB0CTL1 |= UCTXSTT;
				IFG2 &= ~UCB0TXIFG;
				return;
			}
		// no more data to transmit - issue stop
		} else {
			UCB0CTL1 |= UCTXSTP;
			IFG2 &= ~UCB0TXIFG;
			return;
		}
	}
	UCB0TXBUF = *UsciB_data++;
	UsciB_data_counter--;
}

#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCI_Transmit(void)
{
	if (IFG2 & UCB0TXIFG) {
		switch (UCB0CTL0 & UCMODE_3) {
			case UCMODE_0: UsciB_SpiMasterHandler(); break;
			case UCMODE_3: UsciB_I2cTxHandler(); break;
		}
	}
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI_Receive(void)
{
	;
}
