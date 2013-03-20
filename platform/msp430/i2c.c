#include <stdint.h>
#include "bus/i2c.h"
#include "platform/msp430/usci.h"

int I2c_Open(uint8_t device)
{
	return -1;
}

void I2c_Close(int bus)
{
}

int I2c_WriteThenRead(I2cDevice *device, void *tx_buf, uint8_t tx_len, void *rx_buf, uint8_t rx_len)
{
	USCIA_wait();
	return -1;
}

extern int I2c_WriteRegister(I2cDevice *device, uint8_t reg, void *buffer, uint8_t len)
{
	USCIA_wait();
	return -1;
}


