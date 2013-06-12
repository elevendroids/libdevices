/*
*
* Copyright (c) 2013, Michal Potrzebicz <michal@elevendroids.com>
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following 
* conditions are met:
*
*    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*      disclaimer.
*    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*      disclaimer in the documentation and/or other materials provided with the distribution.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
*  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
*  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
*  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/
#include "bus/i2c.h"
#include "gpio/mcp23x17.h"
#include "gpio/mcp23017.h"
#include "display/hd44780.h"
#include "bits.h"
#include "timer.h"
#include "platform/msp430/platform.h"
//TODO: MSP430 specific header - remove
#include <msp430.h>

#include "sensor/bmp085.h"

static void LcdWrite(const uint8_t data, const uint8_t flags);

static const Hd44780Config display_config = {
	.format = HD44780_FORMAT_2x16,
	.data_mode = HD44780_DATA_MODE_8,
	.write_func = LcdWrite
};
static Hd44780Device display 	= { &display_config };

static I2cDevice lcd_adapter 	= { .address = 0x27 };
static I2cDevice sensor		= { .address = 0x77 };

static void LcdWrite(const uint8_t data, const uint8_t flags)
{
	uint8_t device_flags;
	Mcp23x17PortsData lcd_data;

	device_flags = 0;
	if (flags & HD44780_FLAG_E)
		device_flags |= BIT4;
	if (flags & HD44780_FLAG_WR)
		device_flags |= BIT5;
	if (flags & HD44780_FLAG_RS)
		device_flags |= BIT6;

	lcd_data.as_bytes[0] = data;
	lcd_data.as_bytes[1] = device_flags;
	
	Mcp23017_WritePorts(&lcd_adapter, lcd_data);
}

inline static void put_c(char c)
{
	Hd44780_PutChar(&display, c);
}

static const uint32_t uint32_dv[] = {
	1000000000,
	100000000,
	10000000,
	1000000,
	100000,
	// 16 bit
	10000,
	1000,
	100,
	10,
	1
};

static const uint16_t uint16_dv[] = {
	10000,
	1000,
	100,
	10,
	1
};

static void print_uint(uint32_t x)
{
	char c = 0;
	uint16_t i;
	uint32_t d;
	if (x) {
		i = 0;
		do {
			d = uint32_dv[i++];
			if ((x < d) && (c == 0)) continue;
			c = '0';
			while (x >= d) {
				c++;
				x -= d;
			}
			put_c(c);
		} while(!(d & 1));		

	} else {
		put_c('0');
	}
}

static void print_u16(uint16_t x)
{
	char c = 0;
	uint16_t i, d;
	if (x) {
		i = 0;
		do {
			d = uint16_dv[i++];
			if ((x < d) && (c == 0)) continue;		
			c = '0';
			while (x >= d) {
				c++;
				x -= d;
			}
			put_c(c);
		} while(!(d & 1));

	} else {
		put_c('0');
	}
}


void InitDisplay(void)
{
	Hd44780_Init(&display);
	Hd44780_Clear(&display);
}

int main(int argc, char **argvc)
{
	int bus, t;
	uint8_t flags = 0;
	uint16_t vcc;
	int32_t p;
	
	//TODO: MSP430 Launchpad specific code - move to board initialization
	WDTCTL = WDTPW + WDTHOLD;
	P1DIR = 0xFF;
	P1OUT = 0x00;
	P2DIR = 0xFF;
	P2OUT = 0x00;
	P3DIR = 0xFF;
	P3OUT = 0x00;

	BCSCTL3 |= LFXT1S_2;

	Msp430_SetClock(MSP430_CLOCK_1MHZ);
	do {
		Timer_SleepMSec(1000);
		vcc = Msp430_GetSupplyVoltage();
	} while (vcc < 2000);
	Msp430_SetClock(MSP430_CLOCK_8MHZ);

	P1DIR |=  BIT0;
	//

	bus = I2c_Open(0);
	if (bus < 0) {
		return bus;
	}
	lcd_adapter.bus = bus;
	sensor.bus = bus;
	
//	Mcp23017_WriteRegister(&lcd_adapter, MCP23X17_SEQ_IODIRA, 0x00);
//	Mcp23017_WriteRegister(&lcd_adapter, MCP23X17_SEQ_IODIRB, 0x00);

	Bmp085_Init(&sensor);
	do {
		P1OUT |= BIT0;
		
		t = Bmp085_ReadTemperature(&sensor);
		p = Bmp085_ReadPressure(&sensor, 3);
/*		
		vcc = Msp430_GetSupplyVoltage();
		if (vcc >= 3000) {
			if (! (flags & BIT0)) {
				InitDisplay();
				flags |= BIT0;
			};

			Hd44780_SetCursorPos(&display, 0, 0);
			Hd44780_PutString(&display, "T: ");
			print_u16(t / 10);
			Hd44780_PutChar(&display, '.');
			print_u16(t % 10);		
			Hd44780_PutString(&display, "C ");

			Hd44780_SetCursorPos(&display, 1, 0);
			Hd44780_PutString(&display, "P: ");
			print_u16(p / 100);
			Hd44780_PutChar(&display, '.');
			print_u16(p % 100);
			Hd44780_PutString(&display, "hPa ");
		} else if ((flags & BIT0) && (vcc < 2900)) {
			Hd44780_Clear(&display);
			flags &= ~BIT0;
		}
*/		P1OUT &= ~BIT0;
		Timer_SleepSec(30);
	} while (1);
		
	I2c_Close(bus);
	return 0;
}

