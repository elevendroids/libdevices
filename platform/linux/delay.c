#include <unistd.h>

void delay_us(unsigned int useconds)
{
	usleep(useconds);
}

void delay_ms(unsigned int mseconds)
{
	usleep(mseconds * 1000);
}


