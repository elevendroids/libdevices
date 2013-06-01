#include <msp430.h>
#include <stdint.h>
#include "delay.h"
#include "timer.h"
#include "platform/msp430/platform.h"

int ticks_per_ms = 0;
int cycles_per_us = 0;

void Msp430_SetClock(int clock)
{
	uint8_t bcs1, bcs2, dco;
	
	bcs2 = DIVM_0 | DIVS_0;
	
	Timer_Stop();
	__disable_interrupt();

	switch (clock) {
		case MSP430_CLOCK_1MHZ	: 
			bcs1 = CALBC1_1MHZ;
			dco = CALDCO_1MHZ;
			ticks_per_ms = 125;
			cycles_per_us = 1;
			break;
		case MSP430_CLOCK_2MHZ	:
			bcs1 = CALBC1_8MHZ;
			dco = CALDCO_8MHZ;
			// divide SMCLK and MCLK by 4
			bcs2 = DIVM_2 | DIVS_2;
			ticks_per_ms = 250;
			cycles_per_us = 2;
			break;
		case MSP430_CLOCK_4MHZ	:
			bcs1 = CALBC1_8MHZ;
			dco = CALDCO_8MHZ;
			// divide SMCLK and MCLK by 2
			bcs2 = DIVM_1 | DIVS_1;
			ticks_per_ms = 500;
			cycles_per_us = 4;
			break;
		case MSP430_CLOCK_6MHZ	:
			bcs1 = CALBC1_12MHZ;
			dco = CALDCO_12MHZ;
			// divide SMCLK and MCLK by 2
			bcs2 = DIVM_1 | DIVS_1;
			ticks_per_ms = 750;
			cycles_per_us = 6;
			break;
		case MSP430_CLOCK_8MHZ	:
			bcs1 = CALBC1_8MHZ;
			dco = CALDCO_8MHZ;
			ticks_per_ms = 1000;
			cycles_per_us = 8;
			break;
		case MSP430_CLOCK_12MHZ	:
			bcs1 = CALBC1_12MHZ;
			dco = CALDCO_12MHZ;
			ticks_per_ms = 1500;
			cycles_per_us = 12;
			break;
		default:
			bcs1 = CALBC1_16MHZ;
			dco = CALDCO_16MHZ;
			ticks_per_ms = 2000;
			cycles_per_us = 16;
			break;
	}

	// configure DCO
	BCSCTL1 = bcs1;
	// set dividers
	BCSCTL2 = bcs2;
	DCOCTL = dco;
	__enable_interrupt();
	Timer_Start();
}

uint16_t Msp430_GetSupplyVoltage(void)
{
	uint16_t raw_value;
	// first attempt - measure Vcc/2 with 1.5V reference (Vcc < 3V )
	ADC10CTL0 = SREF_1 | REFON | ADC10SHT_2 | ADC10SR | ADC10ON;
	ADC10CTL1 = INCH_11 | SHS_0 | ADC10DIV_0 | ADC10SSEL_0;
	ADC10CTL0 |= ENC | ADC10SC;
	while (ADC10CTL1 & ADC10BUSY) ;
	ADC10CTL0 &= ~(ADC10IFG | ENC);
	raw_value = ADC10MEM;
	// check for overflow
	if (raw_value == 0x3ff) {
		// switch range - use 2.5V reference (Vcc >= 3V)
		ADC10CTL0 = SREF_1 | REF2_5V | REFON | ADC10SHT_2 | ADC10SR | ADC10ON;
		ADC10CTL0 |= ENC | ADC10SC;
		while (ADC10CTL1 & ADC10BUSY) ;
		raw_value = ADC10MEM;
		ADC10CTL0 &= ~(ADC10IFG | ENC);
		// convert value to mV
		return ((uint32_t)raw_value * 5000) / 1024;
	} else
		return ((uint32_t)raw_value * 3000) / 1024;
}

