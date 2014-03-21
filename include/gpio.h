#ifndef _GPIO_H
#define _GPIO_H

#include <stdbool.h>
#include "platform_gpio.h"

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
typedef bool (*gpio_irq_handler_t)(PinId pin_id); 

void pin_init(gpio_pin_t *pin, PinId pin_id, GpioDirection direction);

void pin_init_function(PinId pin_id, int function);

void pin_set_direction(gpio_pin_t *pin, GpioDirection direction);

void pin_set_mode(gpio_pin_t *pin, GpioPinMode mode);

void pin_set(gpio_pin_t *pin, bool state);

bool pin_get(gpio_pin_t *pin);

void pin_toggle(gpio_pin_t *pin);

void gpio_set_irq_handler(gpio_irq_handler_t handler);

void pin_attach_interrupt(gpio_pin_t *pin, GpioIrqEdge mode);

void pin_detach_interrupt(gpio_pin_t *pin);

#endif /* _GPIO_H */
