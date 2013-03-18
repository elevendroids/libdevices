#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bus/i2c.h"

#define I2C_BUS_DEVICE "/dev/i2c-%d"

int I2c_Open(uint8_t device)
{
	char file_name[255];
	snprintf(file_name, sizeof(file_name), I2C_BUS_DEVICE, device);
	return open(file_name, O_RDWR);
}

void I2c_Close(int bus)
{
	close(bus);
}

int I2c_WriteThenRead(I2cDevice *device, void *tx_buf, uint8_t tx_len, void *rx_buf, uint8_t rx_len)
{
	int status;
	struct i2c_rdwr_ioctl_data data;
	struct i2c_msg messages[2];

	messages[0].addr = device->address;
	messages[0].flags = 0;
	messages[0].len = tx_len;
	messages[0].buf = tx_buf;

	messages[1].addr = device->address;
	messages[1].flags = I2C_M_RD;
	messages[1].len = rx_len;
	messages[1].buf = rx_buf;

	data.msgs = messages;
	data.nmsgs = 2;

	status = ioctl(device->bus, I2C_RDWR, &data);

	return status;
}

extern int I2c_WriteRegister(I2cDevice *device, uint8_t reg, void *buffer, uint8_t len)
{
	int status;
	
	uint8_t *buf;
	uint8_t buf_len;
	struct i2c_rdwr_ioctl_data data;
	struct i2c_msg message;

	buf_len = len + sizeof(reg);

	buf = (uint8_t *) malloc(buf_len);
	buf[0] = reg;
	memcpy(&buf[1], buffer, len);
	
	message.addr = device->address;
	message.flags = 0;
	message.len = buf_len;
	message.buf = buf;
	
	data.msgs = &message;
	data.nmsgs = 1;

	status = ioctl(device->bus, I2C_RDWR, &data);

	free(buf);

	return status;
}


