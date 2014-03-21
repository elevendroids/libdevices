#ifndef _PLATFORM_GPIO_H
#define _PLATFORM_GPIO_H

#include <stdint.h>
#include "platform.h"

typedef enum {
	P1_0 = 0x0001,
	P1_1 = 0x0002,
	P1_2 = 0x0004,
	P1_3 = 0x0008,
	P1_4 = 0x0010,
	P1_5 = 0x0020,
	P1_6 = 0x0040,
	P1_7 = 0x0080,

	P2_0 = 0x0101,
	P2_1 = 0x0102,
	P2_2 = 0x0104,
	P2_3 = 0x0108,
	P2_4 = 0x0110,
	P2_5 = 0x0120,
	P2_6 = 0x0140,
	P2_7 = 0x0180,

	P3_0 = 0x0201,
	P3_1 = 0x0202,
	P3_2 = 0x0204,
	P3_3 = 0x0208,
	P3_4 = 0x0210,
	P3_5 = 0x0220,
	P3_6 = 0x0240,
	P3_7 = 0x0280,
	NC = INT16_MAX
} PinId;

typedef struct gpio_tag {
    volatile uint8_t *out;      ///< Output register
    volatile const uint8_t *in; ///< Input register
    volatile uint8_t *dir;      ///< Direction register
    volatile uint8_t *ren;      ///< Pullup/pulldown resistor enable register
    volatile uint8_t *ie;       ///< Interrupt enable register
    volatile uint8_t *ies;      ///< Interrupt mode register
    volatile uint8_t *ifg;      ///< Interrupt flag register
    volatile uint8_t *sel;      ///< Primary peripherial module function select register
    volatile uint8_t *sel2;     ///< Secondary peripherial module function select register
} gpio_t;

typedef struct gpio_pin_tag {
	const gpio_t *port;
	uint8_t mask;
	PinId id;
} gpio_pin_t;

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

#define Gpio_PinMask(pin) (pin & 0xFF)
#define Gpio_PinPort(pin) ((pin >> 8) & 0xFF)
#define Gpio_MakePin(port, pin) ((port << 8) | (pin & 0xFF))

#endif /* _PLATFORM_GPIO_H */
