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
#include <stdbool.h>
#include "bus/i2c.h"
#include "platform/msp430.h"
#include "platform/msp430/usci.h"

bool I2c_Open(uint8_t device, I2cSpeed speed)
{
	UsciB_I2cInit();
	I2c_SetSpeed(speed);
	return 0;
}

void I2c_Close(int bus)
{
}


void I2c_SetSpeed(I2cSpeed speed)
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

bool I2c_Read(I2cDevice *device, uint8_t reg, void *buffer, uint8_t len)
{
	uint8_t msg_count = 0;
	UsciMessage messages[2];

	if (reg != I2C_REGISTER_NONE) {
		messages[msg_count].data = &reg;
		messages[msg_count].length = sizeof(reg);
		messages[msg_count].flags = USCI_MESSAGE_DIR_WRITE;
		msg_count++;
	}

	messages[msg_count].data = buffer;
	messages[msg_count].length = len;
	messages[msg_count].flags = USCI_MESSAGE_DIR_READ;
	msg_count++;

	return UsciB_I2cTransaction(device->address, &messages[0], msg_count);
}

bool I2c_Write(I2cDevice *device, uint8_t reg, void *buffer, uint8_t len)
{
	uint8_t msg_count = 0;
	UsciMessage messages[2];

	if (reg != I2C_REGISTER_NONE) {
		messages[msg_count].data = &reg;
		messages[msg_count].length = sizeof(reg);
		messages[msg_count].flags = USCI_MESSAGE_DIR_WRITE;
		msg_count++;
	}

	messages[msg_count].data = buffer;
	messages[msg_count].length = len;
	messages[msg_count].flags = USCI_MESSAGE_DIR_WRITE | USCI_I2C_NO_RESTART;
	msg_count++;

	return UsciB_I2cTransaction(device->address, &messages[0], msg_count);
}

#endif /* __MSP430_HAS_USCI__ */

