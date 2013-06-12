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


