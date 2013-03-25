#include <msp430.h>

inline void delay_us(unsigned int useconds)
{
	do {
		__delay_cycles(1 * 8);
	} while (--useconds);
}

inline void delay_ms(unsigned int mseconds)
{
	while(mseconds--) {
		__delay_cycles(1000 * 8);
	}
}


