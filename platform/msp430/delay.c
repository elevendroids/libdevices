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
#include <stdint.h>
#include "delay.h"
#include "platform/msp430.h"

DelayFunc Delay_Ms;
DelayFunc Delay_Us;

#define DELAY_US_FUNC(cycles) static void delay_us ## cycles (unsigned int useconds)\
	{ do { __delay_cycles(cycles); } while (--useconds); }

#define DELAY_MS_FUNC(cycles) static void delay_ms ## cycles (unsigned int mseconds)\
	{ do { __delay_cycles(cycles * 1000); } while (--mseconds); }

DELAY_US_FUNC(1);
DELAY_US_FUNC(8);
DELAY_US_FUNC(12);
DELAY_US_FUNC(16);

DELAY_MS_FUNC(1);
DELAY_MS_FUNC(8);
DELAY_MS_FUNC(12);
DELAY_MS_FUNC(16);

static const DelayFunc Delay_ms[] = { delay_ms1, delay_ms8, delay_ms12, delay_ms16 };
static const DelayFunc Delay_us[] = { delay_us1, delay_us8, delay_us12, delay_us16 };

void Delay_Init(void) {
	Delay_Us = Delay_us[Msp430_currentClock];
	Delay_Ms = Delay_ms[Msp430_currentClock];
} 

