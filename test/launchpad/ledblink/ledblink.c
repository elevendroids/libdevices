#include <stdbool.h>
#include "board.h"
#include "delay.h"
#include "gpio.h"

int main(void) {
	gpio_pin_t led;
	Board_Init();
	pin_init(&led, LED1, DIR_OUTPUT);

	while (1) {
		pin_toggle(&led);
		Delay_Ms(500);
	}

	return 0;
}
