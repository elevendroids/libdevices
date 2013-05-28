#include <msp430.h>
#include <stdint.h>

static int ticks_per_ms = 1000;
static volatile uint16_t timer_ticks = 0;

inline void delay_us(unsigned int useconds)
{
	do {
		__delay_cycles(1 * 8);
	} while (--useconds);
}

void delay_ms(unsigned int mseconds)
{
	timer_ticks = mseconds;
	TA0CCR0  = ticks_per_ms;
	TA0CCTL0 = CCIE;
	TA0CTL  = TASSEL_2 | MC_1 | ID_3;
	do {
		LPM1;
	} while (timer_ticks);
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void OnTickTimer(void)
{
	if (timer_ticks) {
		timer_ticks--;
	} else {
		TA0CTL &= ~MC_3;
		LPM1_EXIT;
	}
} 
