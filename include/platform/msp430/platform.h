#ifndef PLATFORM_H
#define PLATFORM_H

#include <stdint.h>

#define MSP430_CLOCK_1MHZ	0x00
#define MSP430_CLOCK_2MHZ	0x01	// 8MHz / 4
#define MSP430_CLOCK_4MHZ	0x02	// 8MHz / 2
#define MSP430_CLOCK_6MHZ	0x04	// 12MHz / 2
#define MSP430_CLOCK_8MHZ	0x05
#define MSP430_CLOCK_12MHZ	0x06
#define MSP430_CLOCK_16MHZ	0x07

extern int ticks_per_ms;
extern int cycles_per_us;

void Msp430_SetClock(int clock);
uint16_t Msp430_GetSupplyVoltage(void);

#endif

