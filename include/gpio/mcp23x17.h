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
#ifndef MCP23X17_H
#define MCP23X17_H

#define MCP23X17_PORTA		0x00
#define MCP23X17_PORTB		0x01

// MCP23x17 registers in sequential mode

// I/O direction ('0' - output, '1' - input)
#define MCP23X17_SEQ_IODIRA	0x00
#define MCP23X17_SEQ_IODIRB	0x01

// Input polarity ('0' - normal, '1' - inversed)
#define MCP23X17_SEQ_IPOLA	0x02
#define MCP23X17_SEQ_IPOLB	0x03

// Interrupt-On-Change control ('0' - disable, '1' - enable)
#define MCP23X17_SEQ_GPINTENA	0x04
#define MCP23X17_SEQ_GPINTENB	0x05

// Default comparision value
#define MCP23X17_SEQ_DEFVALA	0x06
#define MCP23X17_SEQ_DEFVALB	0x07

// Interrupt-On-Change control ('0' - compare with previous, '1' - compare with default)
#define MCP23X17_SEQ_INTCONA	0x08
#define MCP23X17_SEQ_INTCONB	0x09

// Configuration register
// BIT0 - n/a
// BIT1 - polarity of the INT output PIN ('0' - active-low, '1' - active-high)
// BIT2 - INT pin mode ('0' - active, '1' - open drain)
// BIT3 - Hardware address enable (MCP23S17 only) ('0' - disabled, '1' - enabled)
// BIT4 - Slew Rate control for SDA output ('0' - Slew rate enabled, '1' - Slew rate disabled)
// BIT5 - Sequential operation mode ('0' - enabled, '1' - disabled)
// BIT6 - INT pins mirror bit ('0' - INT pins not connected, '1' - INT pins internally connected)
// BIT7 - registers address mode ('0' - sequential, '1' - banked)
#define MCP23X17_SEQ_IOCONA	0x0A
#define MCP23X17_SEQ_IOCONB	0x0B

// Pull-up configuration ('0' - disabled, '1' - enabled)
#define MCP23X17_SEQ_GPPUA	0x0C
#define MCP23X17_SEQ_GPPUB	0x0D

// Interrupt flag ('0' - not pending, '1' - pending)
#define MCP23X17_SEQ_INTFA	0x0E
#define MCP23X17_SEQ_INTFB	0x0F

// Interrupt capture
#define MCP23X17_SEQ_INTCAPA	0x10
#define MCP23X17_SEQ_INTCAPB	0x11

// Port register
#define MCP23X17_SEQ_GPIOA	0x12
#define MCP23X17_SEQ_GPIOB	0x13

// Output latch register
#define MCP23X17_SEQ_OLATA	0x14
#define MCP23X17_SEQ_OLATB	0x15

typedef union {
	uint8_t as_bytes[2];
	uint16_t as_word;
} Mcp23x17PortsData;

#endif
