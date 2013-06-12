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
#ifndef PLATFORM_H
#define PLATFORM_H

#include <stdint.h>

#define MSP430_CLOCK_1MHZ	0x00
#define MSP430_CLOCK_2MHZ	0x01	// 8MHz / 4
#define MSP430_CLOCK_4MHZ	0x02	// 8MHz / 2
#define MSP430_CLOCK_6MHZ	0x03	// 12MHz / 2
#define MSP430_CLOCK_8MHZ	0x04
#define MSP430_CLOCK_12MHZ	0x05
#define MSP430_CLOCK_16MHZ	0x06

extern uint8_t 	Msp430_currentClock;

static const uint16_t _cyclesPerMs[] = {1000, 2000, 4000, 6000, 8000, 12000, 16000};
static const uint8_t  _cyclesPerUs[] = {1, 2, 4, 6, 8, 12, 16};

#define Msp430_cyclesPerMs	_cyclesPerMs[Msp430_currentClock]
#define Msp430_cyclesPerUs	_cyclesPerUs[Msp430_currentClock]

void Msp430_SetClock(int clock);
uint16_t Msp430_GetSupplyVoltage(void);

#endif

