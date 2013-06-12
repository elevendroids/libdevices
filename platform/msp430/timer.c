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
#include "platform/msp430/platform.h"

static volatile unsigned intervalCounter = 0;

void Timer_SleepMSec(unsigned interval)
{
	intervalCounter = interval;
	// set timer period
	TA0CCR0 = Msp430_cyclesPerMs - 1;
	// clock source = SMCLK, up mode, TAIV interrupt enabled
	TA0CTL = TASSEL_2 | MC_1 | TAIE;
	do {
		// sleep with SMCLK running
		_BIS_SR(LPM1_bits | GIE);
	} while (intervalCounter);
	// stop timer, disable interrupt
	TA0CTL &= ~(MC_1 | TAIE);
}

uint16_t Timer_GetVLOFrequency(void)
{
	uint16_t first_capture, ticks_per_vlo;
	uint8_t saved_clock;	
	// save old clock setting
	saved_clock = Msp430_currentClock;

	// switch to 1MHz clock
	Msp430_SetClock(MSP430_CLOCK_1MHZ);
	// set TimerA in continous mode, set capture mode triggered by ACLK
	TA0R = 0x00;
	TA0CCTL0 = CM_1 | CCIS_1 | CAP;
	TA0CTL = TASSEL_2 | ID_0 | MC_2;
	// skip capture
	while (! (TACCTL0 & CCIFG));
	TACCTL0 &= ~CCIFG;
	// get first capture
	while (! (TACCTL0 & CCIFG));
	first_capture = TACCR0;
	TACCTL0 &= ~CCIFG;
	// get second capture
	while (! (TACCTL0 & CCIFG));
	ticks_per_vlo = TACCR0 - first_capture;
	// disable timer
	TA0CTL &= ~MC_1;	
	TA0CCTL0 = 0x00;
	// restore clock
	Msp430_SetClock(saved_clock);
	// return VLO frequency (in Hz)
	return (1000000 / ticks_per_vlo);
}

void Timer_SleepSec(unsigned interval)
{
	intervalCounter = interval;
	// set timer period to ACLK frequency - one interrupt per secnd
	TA0CCR0 = Timer_GetVLOFrequency() - 1;
	// clock source = ACLK, up mode, TAIV interrupt enable
	TA0CTL = TASSEL_1 | MC_1 | TAIE;
	do {
		// deep sleep mode
		_BIS_SR(LPM3_bits | GIE);
	} while (intervalCounter);
	// stop timer and disable interrupt
	TA0CTL &= ~(MC_1 | TAIE);
}

#pragma vector=TIMER0_A1_VECTOR
__interrupt void OnTickTimer(void)
{
	if (TA0IV == TA0IV_TAIFG) {
		intervalCounter--;
		if (intervalCounter == 0) {
			_BIC_SR_IRQ(LPM3_bits); // wake from LPM0-3
		}
	}
} 

