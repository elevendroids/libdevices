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
#include "platform/msp430.h"
#include "platform/msp430/usci.h"

int I2c_Open(uint8_t device, uint8_t speed)
{
	UsciB_I2cInit();
	I2c_SetSpeed(speed);
	return 0;
}

void I2c_Close(int bus)
{
}


void I2c_SetSpeed(uint8_t speed)
{
	// Prescale value = SMCLK(kHz) / SCL(kHz)
	uint16_t prescale;
	if (speed == I2C_SPEED_STANDARD) {
		prescale = (MSP430_CYCLES_PER_MS / 100);
	} else { // there's no highspeed mode available
		prescale = (MSP430_CYCLES_PER_MS / 400);
	}
	UsciB_SetPrescaler(prescale);	
}

int I2c_Read(I2cDevice *device, void *buffer, uint8_t len)
{
	UsciMessage message;

	message.data = buffer;
	message.length = len;
	message.flags = USCI_MESSAGE_DIR_READ;

	return UsciB_I2cTransaction(device->address, &message, 1);
}

int I2c_Write(I2cDevice *device, void *buffer, uint8_t len)
{
	UsciMessage message;

	message.data = buffer;
	message.length = len;
	message.flags = USCI_MESSAGE_DIR_WRITE;

	return UsciB_I2cTransaction(device->address, &message, 1);
}

int I2c_WriteThenRead(I2cDevice *device, void *tx_buf, uint8_t tx_len, void *rx_buf, uint8_t rx_len)
{
	UsciMessage messages[2];

	messages[0].data = tx_buf;
	messages[0].length = tx_len;
	messages[0].flags = USCI_MESSAGE_DIR_WRITE;

	messages[1].data = rx_buf;
	messages[1].length = rx_len;
	messages[1].flags = USCI_MESSAGE_DIR_READ;

	return UsciB_I2cTransaction(device->address, &messages[0], 2);
}

int I2c_WriteThenWrite(I2cDevice *device, void *tx_buf1, uint8_t tx_len1, void *tx_buf2, uint8_t tx_len2)
{
	UsciMessage messages[2];

	messages[0].data = tx_buf1;
	messages[0].length = tx_len1;
	messages[0].flags = USCI_MESSAGE_DIR_WRITE;

	messages[1].data = tx_buf2;
	messages[1].length = tx_len2;
	messages[1].flags = USCI_MESSAGE_DIR_WRITE | USCI_I2C_NO_RESTART;

	return UsciB_I2cTransaction(device->address, &messages[0], 2);
}

#endif /* __MSP430_HAS_USCI__ */

