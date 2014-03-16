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
#include <stdbool.h>
#include "bus/i2c.h"

#define I2C_BUS_DEVICE "/dev/i2c-%d"

bool i2c_open(i2c_t *bus, uint8_t device, i2c_speed_t speed)
{
	char file_name[255];
	snprintf(file_name, sizeof(file_name), I2C_BUS_DEVICE, device);
	bus->handle = open(file_name, O_RDWR);
	return true;
}

void i2c_close(i2c_t *bus)
{
	close(bus->handle);
}

bool i2c_read(i2c_t *bus, uint8_t address, uint8_t reg, void *buffer, uint8_t len)
{
	int msg_count = 0;
	struct i2c_rdwr_ioctl_data data;
	struct i2c_msg messages[2];

	if (reg != I2C_REGISTER_NONE) {
		messages[msg_count].addr = address;
		messages[msg_count].flags = 0;
		messages[msg_count].len = sizeof(reg);
		messages[msg_count].buf = &reg;
		msg_count++;
	}

	messages[msg_count].addr = address;
	messages[msg_count].flags = I2C_M_RD;
	messages[msg_count].len = len;
	messages[msg_count].buf = buffer;
	msg_count++;

	data.msgs = messages;
	data.nmsgs = msg_count;

	return (ioctl(bus->handle, I2C_RDWR, &data) == 0);
}

bool i2c_write(i2c_t *bus, uint8_t address, uint8_t reg, void *buffer, uint8_t len)
{
	int msg_count = 0;
	struct i2c_rdwr_ioctl_data data;
	struct i2c_msg messages[2];

	if (reg != I2C_REGISTER_NONE) {
		messages[msg_count].addr = address;
		messages[msg_count].flags = 0;
		messages[msg_count].len = sizeof(reg);
		messages[msg_count].buf = &reg;
		msg_count++;
	}

	messages[msg_count].addr = address;
	messages[msg_count].flags = 0;
	messages[msg_count].len = len;
	messages[msg_count].buf = buffer;
	msg_count++;

	data.msgs = messages;
	data.nmsgs = msg_count;

	return (ioctl(bus->handle, I2C_RDWR, &data) == 0);
}


