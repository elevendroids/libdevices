/**
 *
 * @file usci_i2c.c
 * @author Michal Potrzebicz
 * @date 2013-07-02
 *
 * @brief USCI I2C bus code, implements I2C support for USCI containing MCU's
 *
 * @section LICENSE
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
#include "digital_io.h"
#include "bus/i2c.h"
#include "platform/msp430.h"
#include "platform/msp430/usci.h"

#define I2C_DIR_READ	0
#define I2C_DIR_WRITE	1

typedef struct {
	const UsciModule *usci;
	uint8_t reg;
	uint8_t *buffer;
	uint8_t len;
	uint8_t dir;
} UsciI2cData;

static bool usci_i2c_rx_handler(UsciI2cData *data)
{
	const UsciModule *usci = data->usci;
	bool result = true;

	if (data->len > 0) {
		data->len--;
		
		*data->buffer++ = *usci->rxbuf;
		// send stop if there's only one byte left
		if (data->len == 1)
			*usci->ctl1 |= UCTXSTP;
		result = (data->len == 0);
	}

	return result;
}

static bool usci_i2c_tx_handler(UsciI2cData *data)
{
	const UsciModule *usci = data->usci;
	bool result = false;

	if (data->reg != I2C_REGISTER_NONE) {
		*usci->txbuf = data->reg;
		data->reg = I2C_REGISTER_NONE;
	}
	else {
		/* send restart if the direction is about to change */
		if (data->dir == I2C_DIR_READ) {
			*usci->ctl1 &= ~UCTR;
			*usci->ctl1 |= UCTXSTT;
			if (data->len == 1) {
				while (*usci->ctl1 & UCTXSTT);
				*usci->ctl1 |= UCTXSTP;
			}
			*usci->ifg &= ~usci->flags.txifg;
		}
		else {
			if (data->len == 0) {
				*usci->ctl1 |= UCTXSTP;
				*usci->ifg &= ~usci->flags.txifg;
				result = true;
			}
			else {
				*usci->txbuf = *data->buffer++;
				data->len--;
			}
		}
	}
	return result;
}

static bool usci_i2c_txrx_handler(const UsciModule *dummy, void *data)
{
	UsciI2cData *xmit_data = (UsciI2cData *)data;
	const UsciModule *usci = xmit_data->usci;

	if (*usci->ifg & usci->flags.rxifg) {
		return usci_i2c_rx_handler(xmit_data);
	}
	else if (*usci->ifg & usci->flags.txifg) {
		return usci_i2c_tx_handler(xmit_data);
	}
	return false;
}

static bool usci_i2c_state_handler(const UsciModule *dummy, void *data)
{
	bool result = false;
	const UsciModule *usci = ((UsciI2cData *)data)->usci;
	if (*usci->stat & UCNACKIFG) {
		*usci->ctl1 |= UCTXSTP;
		*usci->stat &= ~UCNACKIFG;
		result = true;
	}
	return result;
} 


bool i2c_open(i2c_t *bus, uint8_t device, i2c_speed_t speed)
{
	const UsciModule *usci;
	uint16_t prescale;

	if (speed == I2C_SPEED_STANDARD)
		prescale = (MSP430_CYCLES_PER_MS / 100);
	else // there's no highspeed mode available
		prescale = (MSP430_CYCLES_PER_MS / 400);

	Usci_SetHandlers(device, &usci_i2c_txrx_handler, &usci_i2c_state_handler);

	usci = Usci_GetModule(device);
	bus->device = device;
	bus->usci = usci;
	
	Msp430_SetPinFunction(usci->pins.scl, MSP430_PIN_FUNCTION_1 | MSP430_PIN_FUNCTION_2);
	Msp430_SetPinFunction(usci->pins.sda, MSP430_PIN_FUNCTION_1 | MSP430_PIN_FUNCTION_2);

	*usci->ctl1 = UCSSEL_2 | UCSWRST;
	*usci->ctl0 = UCMST | UCMODE_3 | UCSYNC;
	*usci->ctl1 &= ~UCSWRST;

	*usci->br0 = prescale;
	*usci->br1 = 0x00;

	*usci->ie |= usci->flags.txie | usci->flags.rxie;
	*usci->i2cie |= UCNACKIE;

	_EINT();

	return true; 
}

void i2c_close(i2c_t *bus)
{
	const UsciModule *usci = bus->usci;
	*usci->ctl1 = UCSWRST;

	Msp430_SetPinFunction(usci->pins.scl, MSP430_PIN_FUNCTION_NORMAL);
	Msp430_SetPinFunction(usci->pins.sda, MSP430_PIN_FUNCTION_NORMAL);

	Usci_SetHandlers(bus->device, NULL, NULL);
}


void i2c_set_speed(i2c_t *bus, i2c_speed_t speed)
{
	// Prescale value = SMCLK(kHz) / SCL(kHz)
	uint16_t prescale;
	const UsciModule *usci = bus->usci;

	if (speed == I2C_SPEED_STANDARD)
		prescale = (MSP430_CYCLES_PER_MS / 100);
	else // there's no highspeed mode available
		prescale = (MSP430_CYCLES_PER_MS / 400);

	*usci->br0 = prescale;
	*usci->br1 = 0x00;
}

bool i2c_read(i2c_t *bus, uint8_t address, uint8_t reg, void *buffer, uint8_t len)
{
	UsciI2cData data;
	const UsciModule *usci = bus->usci;
	
	while ((*usci->stat & UCBBUSY) != 0);
	data.usci = usci;
	data.reg = reg;
	data.buffer = buffer;
	data.len = len;
	data.dir = I2C_DIR_READ;
	Usci_SetData(bus->device, &data);
	
	*usci->i2csa = address;
	if (reg == I2C_REGISTER_NONE) {
		*usci->ctl1 &= ~UCTR;
		*usci->ctl1 |= UCTXSTT;
		if (len == 1) {
			while (*usci->ctl1 & UCTXSTT);
			*usci->ctl1 |= UCTXSTP;
		}	
	}
	else
		*usci->ctl1 |= UCTR | UCTXSTT;
	
	_DINT();
	if (*usci->stat & UCBBUSY)
		_bis_SR_register(LPM3_bits | GIE);
	else
		_EINT();
//	while (UCB0STAT & UCBBUSY) ;

	Usci_SetData(bus->device, NULL);
	return (data.len == 0);
}

bool i2c_write(i2c_t *bus, uint8_t address, uint8_t reg, void *buffer, uint8_t len)
{
	UsciI2cData data;
	const UsciModule *usci = bus->usci;

	while (*usci->stat & UCBBUSY);
	data.usci = usci;
	data.reg = reg;
	data.buffer = buffer;
	data.len = len;
	data.dir = I2C_DIR_WRITE;
	Usci_SetData(bus->device, &data);
	
	*usci->i2csa = address;
	*usci->ctl1 |= UCTR | UCTXSTT;
	
	_DINT();
	if (*usci->stat & UCBBUSY)
		_bis_SR_register(LPM3_bits | GIE);
	else
		_EINT();
//	while (UCB0STAT & UCBBUSY) ;

	Usci_SetData(bus->device, NULL);
	return (data.len == 0);

}

#endif /* __MSP430_HAS_USCI__ */

