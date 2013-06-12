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

inline void delay_us(unsigned int useconds)
{
	do {
		switch (Msp430_cyclesPerUs) {
			case 1 : __delay_cycles(1); break;
			case 2 : __delay_cycles(2); break;
			case 4 : __delay_cycles(4); break;
			case 6 : __delay_cycles(6); break;
			case 8 : __delay_cycles(8); break;
			case 12: __delay_cycles(12); break;
			case 16: __delay_cycles(16); break;
		}
	} while (--useconds);
}

void delay_ms(unsigned int mseconds)
{
	do {
		switch (Msp430_cyclesPerUs) {
			case 1 : __delay_cycles(1 * 1000); break;
			case 2 : __delay_cycles(2 * 1000); break;
			case 4 : __delay_cycles(4 * 1000); break;
			case 6 : __delay_cycles(6 * 1000); break;
			case 8 : __delay_cycles(8 * 1000); break;
			case 12: __delay_cycles(12 * 1000); break;
			case 16: __delay_cycles(16 * 1000); break;
		}
	} while (--mseconds);
} 

