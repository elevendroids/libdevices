#include <stdint.h>
#include "bus/i2c.h"
#include "platform/msp430/usci.h"


int I2c_Open(uint8_t device)
{
	USCIB_I2cInit(20);
	return 0;
}

void I2c_Close(int bus)
{
}

int I2c_Read(I2cDevice *device, void *buffer, uint8_t len)
{
	UsciMessage message;
	UsciTransaction transaction = { .messages = &message, .message_count = 1 };

	message.data = buffer;
	message.length = len;
	message.flags = USCI_MESSAGE_DIR_READ;

	return UsciB_I2cTransaction(device->address, &transaction);
}

int I2c_Write(I2cDevice *device, void *buffer, uint8_t len)
{
	UsciMessage message;
	UsciTransaction transaction = { .messages = &message, .message_count = 1 };

	message.data = buffer;
	message.length = len;
	message.flags = USCI_MESSAGE_DIR_WRITE;

	return UsciB_I2cTransaction(device->address, &transaction);
}

int I2c_WriteThenRead(I2cDevice *device, void *tx_buf, uint8_t tx_len, void *rx_buf, uint8_t rx_len)
{
	UsciMessage messages[2];
	UsciTransaction transaction = { .messages = &messages[0], .message_count = 2 };

	messages[0].data = tx_buf;
	messages[0].length = tx_len;
	messages[0].flags = USCI_MESSAGE_DIR_WRITE;

	messages[1].data = rx_buf;
	messages[1].length = rx_len;
	messages[1].flags = USCI_MESSAGE_DIR_READ;

	return UsciB_I2cTransaction(device->address, &transaction);
}

int I2c_WriteRegister(I2cDevice *device, uint8_t reg, void *buffer, uint8_t len)
{
	UsciMessage messages[2];
	UsciTransaction transaction = { .messages = &messages[0], .message_count = 2 };

	messages[0].data = &reg;
	messages[0].length = sizeof(reg);
	messages[0].flags = USCI_MESSAGE_DIR_WRITE;

	messages[1].data = buffer;
	messages[1].length = len;
	messages[1].flags = USCI_MESSAGE_DIR_WRITE | USCI_I2C_NO_RESTART;

	return UsciB_I2cTransaction(device->address, &transaction);
}


