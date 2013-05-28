#include <msp430.h>
#include <stdint.h>
#include "platform/msp430/usci.h"

#define USCI_MODE_I2C	BIT0
#define USCI_MODE_SPI	BIT1

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
	IE2 |= UCB0TXIE | UCB0RXIE;
//	UCB0I2CIE |= UCNACKIE;
}

int UsciB_I2cTransaction(uint8_t address, UsciTransaction *transaction)
{
	while (UCB0STAT & UCBBUSY);
	// Set slave address
	UCB0I2CSA = address;
	//

	UsciB_message = transaction->messages;
	UsciB_message_counter = transaction->message_count - 1;
	UsciB_data = UsciB_message->data;
	UsciB_data_counter = UsciB_message->length;

	if (UsciB_message->flags & USCI_MESSAGE_DIR_READ) {
		UCB0CTL1 &= ~UCTR;
		UCB0CTL1 |= UCTXSTT;
		if ((UsciB_message_counter == 0) && (UsciB_data_counter == 1)) {
			while (UCB0CTL1 & UCTXSTT);
			UCB0CTL1 |= UCTXSTP;
		}
	} else {
		UCB0CTL1 |= UCTR + UCTXSTT;
	}
	LPM3;
	while (UCB0STAT & UCBBUSY);
	return 0;
}

static inline void UsciB_SpiMasterHandler()
{
	_NOP();
}

void UsciB_I2cRxHandler(void)
{
	volatile uint8_t *data;
	UsciB_data_counter--;
	data = UsciB_data++; // save current data pointer

	if (UsciB_data_counter == 0) {
		// fetch next message, if any
		if (UsciB_message_counter) {
			UsciB_message_counter--;
			UsciB_message++;
			UsciB_data = UsciB_message->data;
			UsciB_data_counter = UsciB_message->length;

			// check next message's direction
			if (UsciB_message->flags & USCI_MESSAGE_DIR_READ) {
				// set RESTART condition if needed
				if ( !(UsciB_message->flags & USCI_I2C_NO_RESTART)) {
					UCB0CTL1 |= UCTXSTT;
					// set STOP if there's only one byte to read
					if ((UsciB_message_counter == 0) && (UsciB_data_counter == 1)) {
						while (UCB0CTL1 & UCTXSTT);
						UCB0CTL1 |= UCTXSTP;
					}
				}
			// switch to write mode and restart
			} else {
				UCB0CTL1 |= UCTR | UCTXSTT;
			}
		} else {
			// no more data to read
		}
	// if there's only one byte left to read, set STOP condition
	} else if ((UsciB_data_counter == 1) && (UsciB_message_counter == 0)) {
		UCB0CTL1 |= UCTXSTP;
	}
 
	//read data byte, clear UCB0RXIFG, apply any of the conditions set above
	*data = UCB0RXBUF;
}

void UsciB_I2cTxHandler(void) 
{
	if (UsciB_data_counter == 0) {
		// fetch next message, if any
		if (UsciB_message_counter) {
			UsciB_message_counter--;
			UsciB_message++;
			UsciB_data = UsciB_message->data;
			UsciB_data_counter = UsciB_message->length;

			// if direction has changed, issue restart in read mode and exit
			if (UsciB_message->flags & USCI_MESSAGE_DIR_READ) {
				UCB0CTL1 &= ~UCTR;
				UCB0CTL1 |= UCTXSTT;
				if ((UsciB_message_counter == 0) && (UsciB_data_counter == 1)) {
					while (UCB0CTL1 & UCTXSTT);
					UCB0CTL1 |= UCTXSTP;
				}
				IFG2 &= ~UCB0TXIFG;
				return;
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
	if (IFG2 & UCB0RXIFG) {
		switch (UCB0CTL0 & UCMODE_3) {
			case UCMODE_3: UsciB_I2cRxHandler(); break;
		}
	} else if (IFG2 & UCB0TXIFG) {
		switch (UCB0CTL0 & UCMODE_3) {
			case UCMODE_0: UsciB_SpiMasterHandler(); break;
			case UCMODE_3: UsciB_I2cTxHandler(); break;
		}
	}
	if ((UsciB_data_counter == 0) && (UsciB_message_counter == 0))
		LPM3_EXIT;
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI_Receive(void)
{
	
}
