#ifndef _GPIO_H
#define _GPIO_H

#include "platform_gpio.h"
#include "gpio_pin.h"

typedef enum {
	DIR_INPUT,
	DIR_OUTPUT
} GpioDirection;

typedef enum {
	MODE_NORMAL,
	MODE_PULLUP,
	MODE_PULLDOWN
} GpioPinMode;

typedef enum {
	EDGE_FALLING,
	EDGE_RISING,
	EDGE_CHANGE
} GpioIrqEdge;

typedef void (*PinIntCallback)(void);

void pin_init(gpio_pin_t *pin, GpioPin pin_id);

void pin_init_function(GpioPin pin_id, int function);

void pin_set_direction(gpio_pin_t *pin, GpioDirection direction);

void pin_set_mode(gpio_pin_t *pin, GpioPinMode mode);

void pin_set(gpio_pin_t *pin, bool state);

bool pin_get(gpio_pin_t *pin);

void pin_toggle(gpio_pin_t *pin);

#endif /* _GPIO_H */
