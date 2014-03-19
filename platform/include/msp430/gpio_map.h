#ifndef _GPIO_MAP_H
#define _GPIO_MAP_H

#include "platform/msp430.h"
#include "gpio.h"

static const gpio_t P1 = {
	&P1OUT,
	&P1IN,
	&P1DIR,
	&P1REN,
	&P1IE,
	&P1IES,
	&P1IFG,
	&P1SEL,
	&P1SEL2
};

#ifdef MSP430_HAS_PORT2
static const gpio_t P2 = {
	&P2OUT,
	&P2IN,
	&P2DIR,
	&P2REN,
	&P2IE,
	&P2IES,
	&P2IFG,
	&P2SEL,
	&P2SEL2
};
#endif

#ifdef MSP430_HAS_PORT3
static const gpio_t P3 = {
	&P3OUT,
	&P3IN,
	&P3DIR,
	&P3REN,
	.sel = &P3SEL,
	.sel2 = &P3SEL2
};
#endif

static const gpio_t *gpio_ports[] = { 
	 &P1
#ifdef MSP430_HAS_PORT2
	,&P2
#endif
#ifdef MSP430_HAS_PORT3
	,&P3
#endif
};

#endif /* _GPIO_MAP_H */
