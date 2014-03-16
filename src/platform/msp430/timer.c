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
#include <stdbool.h>
#include <limits.h>
#include "platform/msp430.h"

volatile static unsigned int miliseconds;
volatile static unsigned int sleep_ms;

#ifndef USE_EXTERNAL_CRYSTAL
#define USE_EXTERNAL_CRYSTAL 0
#endif

#if (USE_EXTERNAL_CRYSTAL == 1)
static const uint16_t us_per_tick = 1953;

void Timer_Init(void) 
{
	BCSCTL3 = LFXT1S_0 | XCAP_3;
	WDTCTL = WDT_ADLY_1_9;
	IE1 |= WDTIE;
}
#else
static uint16_t us_per_tick;

static uint16_t vlo_us_per_tick(void) {
	uint16_t cycles;
	
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;

	TA0CTL = TASSEL_2 | ID_0 | TACLR;
	TA0CCTL0 = CM_1 | CCIS_1 | CAP | SCS;
	TA0CTL |= MC_2;

	while ((TA0CCTL0 & CCIFG) == 0);
	TA0CCR0 = 0;
	TA0CCTL0 &= ~CCIFG;

	while ((TA0CCTL0 & CCIFG) == 0);
	cycles = TA0CCR0;
	TA0CCTL0 &= ~CCIFG;

	while ((TA0CCTL0 & CCIFG) == 0);
	cycles = TA0CCR0 - cycles;
	TA0CCTL0 &= ~CCIFG;

	TA0CTL = TACLR;

	Msp430_InitClock();

	return (cycles * 64);
}

void Timer_Init(void) 
{
	BCSCTL3 = LFXT1S_2;
	us_per_tick = vlo_us_per_tick();
	WDTCTL = WDT_ADLY_1_9;
	IE1 |= WDTIE;
}
#endif

unsigned int Timer_GetTicks(void) 
{
	return miliseconds;
}

unsigned int Timer_ElapsedTicks(unsigned int start, unsigned int end) {
	if (start > end)
		return end + (UINT_MAX - start);
	else
		return end - start;
}

bool Timer_IsTimeOut(unsigned int ticks, unsigned int timeout) {
	unsigned int current_ticks = Timer_GetTicks();
	unsigned int elapsed_ticks;

	elapsed_ticks = Timer_ElapsedTicks(ticks, current_ticks);

	return (elapsed_ticks >= timeout);
}

void Timer_Sleep(unsigned int ticks) {
	unsigned int interval;
	uint8_t ms_per_tick;
	
	ms_per_tick = (us_per_tick / 1000);

	if (ticks <= ms_per_tick)
		interval = ms_per_tick + 1;
	else
		interval = ticks;

	_DINT();
	sleep_ms = interval;
	_bis_SR_register(LPM3_bits | GIE);

/*
	while (sleep_ms) {
		_DINT();
		if (sleep_ms)
			_bis_SR_register(LPM3_bits | GIE);
		else
			_EINT();
	}
*/
}

#pragma vector=WDT_VECTOR
__interrupt void Watchdog_ISR(void)
{
	static unsigned int microseconds = 0;
	unsigned int sleep_counter;
	unsigned int elapsed_ms = 0;
	
	microseconds += us_per_tick;
	while (microseconds >= 1000 ) {
		elapsed_ms++;
		microseconds -= 1000;
	}

	sleep_counter = sleep_ms;
	if (sleep_counter) {
		if (sleep_counter >= elapsed_ms)
			sleep_counter -= elapsed_ms;
		else
			sleep_counter = 0;
		
		if (sleep_counter == 0)
			_bic_SR_register_on_exit(LPM3_bits);
		
		sleep_ms = sleep_counter;
	}

	miliseconds += elapsed_ms;
}


