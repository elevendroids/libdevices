#include <msp430.h>
#include "platform/msp430/platform.h"

#define TIMER_WAKE_UP	BIT0

static volatile unsigned timer_ticks	= 0;
static volatile unsigned wake_up_ticks 	= 0;
static volatile unsigned timer_flags 	= 0;


void Timer_Start(void)
{
	TA0CCR0 = ticks_per_ms;
	TA0CCTL0 = CCIE;
	TA0CTL = TASSEL_2 | MC_1 | ID_3;
}

void Timer_Stop(void)
{
	TA0CTL &= ~MC_3;
}

void Timer_Sleep(unsigned interval)
{
	wake_up_ticks = interval;
	timer_flags |= TIMER_WAKE_UP;
	do {
		LPM1;
	} while (wake_up_ticks);
}

unsigned Timer_GetTicks(void)
{
	return timer_ticks;
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void OnTickTimer(void)
{
	timer_ticks++;
	if ( timer_flags & TIMER_WAKE_UP) {
		if (wake_up_ticks) {
			wake_up_ticks--;
		} else {
			timer_flags &= ~TIMER_WAKE_UP;
			LPM1_EXIT;
		}
	}
}

