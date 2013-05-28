#include "bus/i2c.h"
#include "gpio/mcp23x17.h"
#include "gpio/mcp23017.h"
#include "display/hd44780.h"
#include "bits.h"
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

int main(int argc, char **argvc)
{
	int bus, t;
	int32_t p;
	
	//TODO: MSP430 Launchpad specific code - move to board initialization
	WDTCTL = WDTPW + WDTHOLD;
	BCSCTL1 = CALBC1_8MHZ;
        DCOCTL = CALDCO_8MHZ;
	__enable_interrupt();
	//

	bus = I2c_Open(0);
	if (bus < 0) {
		return bus;
	}
	lcd_adapter.bus = bus;
	sensor.bus = bus;

	
	Mcp23017_WriteRegister(&lcd_adapter, MCP23X17_SEQ_IODIRA, 0x00);
	Mcp23017_WriteRegister(&lcd_adapter, MCP23X17_SEQ_IODIRB, 0x00);

	Hd44780_Init(&display);
	Hd44780_Clear(&display);

	Bmp085_Init(&sensor);
	do {
		Hd44780_SetCursorPos(&display, 0, 0);
		Hd44780_PutString(&display, "T: ");
		t = Bmp085_ReadTemperature(&sensor);
		print_u16(t / 10);
		Hd44780_PutChar(&display, '.');
		print_u16(t % 10);		
		Hd44780_PutString(&display, "C ");

		Hd44780_SetCursorPos(&display, 1, 0);
		Hd44780_PutString(&display, "P: ");
		p = Bmp085_ReadPressure(&sensor, 3);
		print_u16(p / 100);
		Hd44780_PutChar(&display, '.');
		print_u16(p % 100);
		Hd44780_PutString(&display, "hPa ");
		delay_ms(1000);
	} while (1);
		
	I2c_Close(bus);
	return 0;
}

