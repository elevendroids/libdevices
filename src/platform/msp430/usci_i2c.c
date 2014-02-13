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
#include <stddef.h>
#include "bus/i2c.h"
#include "platform/msp430.h"
#include "platform/msp430/usci.h"

#define I2C_DIR_READ	0
#define I2C_DIR_WRITE	1

typedef struct {
	uint8_t reg;
	uint8_t *buffer;
	uint8_t len;
	uint8_t dir;
} UsciI2cData;

bool UsciB0_I2cXmitHandler(void *data)
{
	UsciI2cData *xmit_data = (UsciI2cData *)data;
	if (IFG2 & UCB0RXIFG) {
		if (xmit_data->len) {
			xmit_data->len--;
			if (xmit_data->len == 1)
				UCB0CTL1 |= UCTXSTP;
			*xmit_data->buffer++ = UCB0RXBUF;
		}
		else
			return true;
	}
	else if (IFG2 & UCB0TXIFG) {
		if (xmit_data->reg != I2C_REGISTER_NONE) {
			UCB0TXBUF = xmit_data->reg;
			xmit_data->reg = I2C_REGISTER_NONE;
		}
		else {
			if (xmit_data->dir == I2C_DIR_READ) {
				UCB0CTL1 &= ~UCTR;
				UCB0CTL1 |= UCTXSTT;
				if (xmit_data->len == 1) {
					while (UCB0CTL1 & UCTXSTT);
					UCB0CTL1 |= UCTXSTP;
				}
				IFG2 &= ~UCB0TXIFG;
			}
			else {
				if (xmit_data->len == 0) {
					UCB0CTL1 |= UCTXSTP;
					IFG2 &= ~UCB0TXIFG;
					return true;
				}
				else {
					UCB0TXBUF = *xmit_data->buffer++;
					xmit_data->len--;
				}
			}
		}
	}
	return false;
}

static bool I2c_RxHandler(void *data)
{
	if (UCB0STAT & UCNACKIFG) {
		UCB0CTL1 |= UCTXSTP;
		UCB0STAT &= ~UCNACKIFG;
		return true;
	}
	return false;
} 

int I2c_Open(uint8_t device, I2cSpeed speed)
{
	Usci_SetHandlers(device, &UsciB0_I2cXmitHandler, &I2c_RxHandler);
	UCB0CTL1 = UCSSEL_2 + UCSWRST;
	UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;
	UCB0CTL1 &= ~UCSWRST;

	I2c_SetSpeed(device, speed);
	IE2 |= UCB0TXIE | UCB0RXIE;
	UCB0I2CIE |= UCNACKIE;
	
	_EINT();

	return device; 
}

void I2c_Close(int bus)
{
	Usci_SetHandlers(bus, NULL, NULL);
}


void I2c_SetSpeed(int bus, I2cSpeed speed)
{
	// Prescale value = SMCLK(kHz) / SCL(kHz)
	uint16_t prescale;

	if (speed == I2C_SPEED_STANDARD)
		prescale = (MSP430_CYCLES_PER_MS / 100);
	else // there's no highspeed mode available
		prescale = (MSP430_CYCLES_PER_MS / 400);

	UCB0BR0 = prescale;
	UCB0BR1 = 0x00;
}

bool I2c_Read(I2cDevice *device, uint8_t reg, void *buffer, uint8_t len)
{
	UsciI2cData data;
	while (UCB0STAT & UCBBUSY) ;
	data.reg = reg;
	data.buffer = buffer;
	data.len = len;
	data.dir = I2C_DIR_READ;
	Usci_SetData(device->bus, &data);
	
	UCB0I2CSA = device->address;
	if (reg == I2C_REGISTER_NONE) {
		UCB0CTL1 &= ~UCTR;
		UCB0CTL1 |= UCTXSTT;
		if (len == 1) {
			while (UCB0CTL1 & UCTXSTT);
			UCB0CTL1 |= UCTXSTP;
		}	
	}
	else
		UCB0CTL1 |= UCTR | UCTXSTT;
	
	_DINT();
	if (UCB0STAT & UCBBUSY)
		_bis_SR_register(LPM3_bits | GIE);
	else
		_EINT();
//	while (UCB0STAT & UCBBUSY) ;

	Usci_SetData(device->bus, NULL);
	return (data.len == 0);
}

bool I2c_Write(I2cDevice *device, uint8_t reg, void *buffer, uint8_t len)
{
	UsciI2cData data;
	while (UCB0STAT & UCBBUSY) ;
	data.reg = reg;
	data.buffer = buffer;
	data.len = len;
	data.dir = I2C_DIR_WRITE;
	Usci_SetData(device->bus, &data);
	
	UCB0I2CSA = device->address;
	UCB0CTL1 |= UCTR | UCTXSTT;
	
	_DINT();
	if (UCB0STAT & UCBBUSY)
		_bis_SR_register(LPM3_bits | GIE);
	else
		_EINT();
//	while (UCB0STAT & UCBBUSY) ;

	Usci_SetData(device->bus, NULL);
	return (data.len == 0);

}

#endif /* __MSP430_HAS_USCI__ */

