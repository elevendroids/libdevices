#ifndef _PLATFORM_GPIO_H
#define _PLATFORM_GPIO_H

#include <stdint.h>

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
} gpio_pin_t;

#endif /* _PLATFORM_GPIO_H */
