#include "board.h"
#include "delay.h"
#include "digital_io.h"
#include "board/mspnode.h"

int main(void) {
	Board_Init();

	while (1) {
		Pin_Toggle(LED_ERROR);
		Delay_Ms(500);
		Pin_Toggle(LED_STATUS_1);
		Delay_Ms(500);
		Pin_Toggle(LED_STATUS_2);
		Delay_Ms(500);
	}
	
	return 0;
}
