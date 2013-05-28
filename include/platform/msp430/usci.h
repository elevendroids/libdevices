#ifndef USCI_H
#define USCI_H

#define TX_BUFFER_LEN 16

// direction flags
// SPI can simultanously transmit and receive data, 
// so we allow to mark messages as write/read - buffer will be filled with received data
#define USCI_MESSAGE_DIR	0x03
#define USCI_MESSAGE_DIR_READ 	0x01
#define USCI_MESSAGE_DIR_WRITE	0x02

// by flagging I2C message as no-restart we can simply append data buffers to each other
// otherwise, a restart condition will be send before the message
#define USCI_I2C_NO_RESTART	0x04

// I2C - append next message
// SPI - keep device selected between messages
#define USCI_MESSAGE_PARTIAL	0x04

// structures for synchronous operation (SPI, I2C)
typedef struct {
	uint8_t *data;
	uint8_t length;
	uint8_t flags;	
} UsciMessage;

typedef struct {
	UsciMessage *messages;
	uint8_t message_count;
} UsciTransaction;
//


void USCIB_I2cInit(uint8_t prescale);

int UsciB_I2cTransaction(uint8_t address, UsciTransaction *transaction);
int UsciB_SpiTransaction(UsciTransaction *transaction , uint8_t count);

void USCIA_Wait(void);
void USCIB_Wait(void);

#endif
