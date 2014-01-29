#include "board.h"
#include "timer.h"
#include "digital_io.h"
#include "board/launchpad.h"

int main(void) {
	Board_Init();
	Timer_Init();
	Pin_SetMode(LED1, PIN_MODE_OUTPUT);

	while (1) {
		Timer_Sleep(500);
		Pin_Toggle(LED1);
	}
	
	return 0;
}
