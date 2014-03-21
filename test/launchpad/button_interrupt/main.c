#include <stdbool.h>
#include "board.h"
#include "delay.h"
#include "gpio.h"

gpio_pin_t led;
gpio_pin_t button;

bool on_gpio_irq(PinId pin)
{
	if (pin == BUTTON)
		pin_toggle(&led);
	return false;
}

int main(void) {
	Board_Init();
	pin_init(&led, LED1, DIR_OUTPUT);
	pin_init(&button, BUTTON, DIR_INPUT);
	pin_set_mode(&button, MODE_PULLUP);
	pin_set(&led, true);

	gpio_set_irq_handler(&on_gpio_irq);	
	pin_attach_interrupt(&button, EDGE_FALLING);

	LPM3;
	
	return 0;
}
