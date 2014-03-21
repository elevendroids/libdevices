/**
 *
 * @file usci.h
 * @author Michal Potrzebicz
 * @date 2013-03-20
 *
 * @brief Common USCI API for MSP430 microcontrollers
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
#ifndef USCI_H
#define USCI_H

#include <msp430.h>
#include <stdbool.h>
#include <stdint.h>
#include "platform.h"
#include "gpio.h"

#define USE_SERIAL_FIFO		1

#define USCI_MODULE_COUNT	2

#define USCI_A0				0
#define USCI_B0				1

//TODO:
//#define USCI_A1				2
//#define USCI_B1				3

/**
 * Structure defining an USCI module
 */
typedef struct {
	volatile uint8_t *ctl0;			///< Control register 0
	volatile uint8_t *ctl1;			///< Control register 1
	volatile uint8_t *br0;			///< Baud rate control register 0
	volatile uint8_t *br1;			///< Baud rate control register 1
	volatile uint8_t *mctl;			///< Modulation control register (UART)
	volatile uint8_t *stat;			///< Status register
	volatile const uint8_t *rxbuf;	///< Receive buffer
	volatile uint8_t *txbuf;		///< Transmit buffer
	volatile uint8_t *abctl;		///< Auto baud control register (UART)
	volatile uint8_t *irtctl;		///< IrDA transmit control register (UART)
	volatile uint8_t *irrctl;		///< IrDA receive control register (UART)
	volatile uint8_t *i2cie;		///< I2C interrupt enable register (I2C)
	volatile uint16_t *i2coa;		///< I2C own address register (I2C)
	volatile uint16_t *i2csa;		///< I2C slave address register (I2C)
	
	volatile uint8_t *ie;			///< Interrupt enable register
	volatile uint8_t *ifg;			///< Interrupt flag register

	/// Module interrupt flags	
	struct {
		const uint8_t rxifg;		///< Receive interrupt flag
		const uint8_t txifg;		///< Transmit interrupt flag
		const uint8_t rxie;			///< Receive interrupt enable
		const uint8_t txie;			///< Transmit interrupt enable
	} flags;

	/// Module pins (MCU-specific)
	struct {
		const PinId scl;			///< I2C SCL pin
		const PinId sda;			///< I2C SDA pin
		const PinId somi;			///< SPI data in pin
		const PinId simo;			///< SPI data out pin
		const PinId ste;			///< SPI slave transmit enable pin
		const PinId clk;			///< SPI clock pin
		const PinId rxd;			///< UART RX pin
		const PinId txd;			///< UART TX pin
	} pins;
} UsciModule;

/**
 * USCI interrupt handler
 * @param usci USCI module generating the interrupt
 * @param data USCI module data - mode specific
 * @returns true if transaction has finished and we can wake up the MCU
 */
typedef bool (*UsciHandler)(const UsciModule *usci, void *data);

typedef struct {
	UsciHandler tx_handler; ///< Transmit handler. Note, that for I2C it's called for both TX and RX
	UsciHandler rx_handler; ///< Receive handler. Note, that for I2C it handles bus state change interrupts
	void *data;				///< Bus-specific data
} UsciData;

/**
 * @param usci - Index of requested USCI module
 * @returns Pointer to UsciModule structure
 */
const UsciModule* Usci_GetModule(int usci);

/**
 * @param usci - Index of USCI module
 * @param tx_handler - Transmit interrupt handler; in I2C mode will also be called for received data
 * @param rx_handler - Receive interrupt handler; in I2C will be called for bus state change interrupts
 */
void Usci_SetHandlers(int usci, UsciHandler tx_handler, UsciHandler rx_handler);

/**
 * @param usci - Index of USCI module
 * @param data - Data which will be provided to TX/RX handlers
 */
void Usci_SetData(int usci, void *data);

#endif
