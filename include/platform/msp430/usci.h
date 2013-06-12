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
#ifndef USCI_H
#define USCI_H

#define TX_BUFFER_LEN 16

// direction flags
// SPI can simultanously transmit and receive data, 
// so we allow to mark messages as write/read - buffer will be filled with received data
#define USCI_MESSAGE_DIR	0x03
#define USCI_MESSAGE_DIR_READ 	0x01
#define USCI_MESSAGE_DIR_WRITE	0x02

// by flagging I2C message as no-restart we can simply append data buffers to each other
// otherwise, a restart condition will be send before the message
#define USCI_I2C_NO_RESTART	0x04

// I2C - append next message
// SPI - keep device selected between messages
#define USCI_MESSAGE_PARTIAL	0x04

// structures for synchronous operation (SPI, I2C)
typedef struct {
	uint8_t *data;
	uint8_t length;
	uint8_t flags;	
} UsciMessage;

typedef struct {
	UsciMessage *messages;
	uint8_t message_count;
} UsciTransaction;
//


void USCIB_I2cInit(uint8_t prescale);

int UsciB_I2cTransaction(uint8_t address, UsciTransaction *transaction);
int UsciB_SpiTransaction(UsciTransaction *transaction , uint8_t count);

void USCIA_Wait(void);
void USCIB_Wait(void);

#endif
