#include "board.h"
#include "delay.h"
#include "digital_io.h"
#include "board/launchpad.h"

int main(void) {
	Board_Init();
	Pin_SetMode(LED1, PIN_MODE_OUTPUT);
	Pin_SetMode(LED2, PIN_MODE_OUTPUT);

	while (1) {
		Pin_Toggle(LED1);
		Delay_Ms(500);
		Pin_Toggle(LED2);
		Delay_Ms(500);
	}
	
	return 0;
}
