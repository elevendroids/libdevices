#include "board.h"
#include "delay.h"
#include "digital_io.h"
#include "board/launchpad.h"

void on_button_int() {
	Pin_Toggle(LED1);
}

int main(void) {
	Board_Init();
	Pin_SetMode(LED1, PIN_MODE_OUTPUT);
	Pin_SetMode(BUTTON, PIN_MODE_INPUT | PIN_MODE_PULLUP);
	Pin_AttachInterrupt(BUTTON, &on_button_int, PIN_INT_MODE_FALLING);

	while (1);
	
	return 0;
}
