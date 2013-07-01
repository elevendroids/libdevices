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

#include <msp430.h>

#ifdef __MSP430_HAS_USCI__

#include <stdint.h>
#include "bus/i2c.h"
#include "platform/msp430/platform.h"
#include "platform/msp430/usci.h"


int I2c_Open(uint8_t device, uint8_t speed)
{
	// This code is optimized to use one simple division operation (inlined by the compiler).
	// Prescale value = SMCLK(kHz) / SCL(kHz)
	uint16_t temp;
	if (speed == I2C_SPEED_STANDARD) {
		temp = Msp430_cyclesPerMs; // leave as is for 100kHz
	} else { // there's no highspeed mode available
		temp = (Msp430_cyclesPerMs * 4); // multiply by 4 for 400kHz
	}
	USCIB_I2cInit(temp / 100);
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

#endif

