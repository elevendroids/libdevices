#include "bus/i2c.h"
#include "gpio/mcp23x17.h"
#include "gpio/mcp23017.h"
#include "display/hd44780.h"
#include <unistd.h>
#include <stdio.h>
#include "bits.h"

static void LcdWrite(uint8_t data, uint8_t flags);

static const Hd44780Config display_config = {
	.lines = 2,
	.columns = 16,
	.data_mode = HD44780_DATA_MODE_8,
	.write_func = &LcdWrite
};
static Hd44780Device display = { &display_config };

static I2cDevice lcd_adapter = { .address = 0x27 };

static void LcdWrite(uint8_t data, uint8_t flags)
{
	uint8_t device_flags;
	Mcp23x17Data lcd_data;

	device_flags = 0;
	if (flags & HD44780_FLAG_E)
		device_flags |= BIT4;
	if (flags & HD44780_FLAG_WR)
		device_flags |= BIT5;
	if (flags & HD44780_FLAG_RS)
		device_flags |= BIT6;

	lcd_data.bytes[0] = data;
	lcd_data.bytes[1] = device_flags | BIT7;
	Mcp23017_WriteData(&lcd_adapter, lcd_data);
}

int main(int argc, char **argvc)
{
	int bus;

	bus = I2c_Open(0);
	if (bus < 0) {
		return bus;
	}
	lcd_adapter.bus = bus;
	
	Mcp23017_WriteRegister(&lcd_adapter, MCP23X17_SEQ_IODIRA, 0x00);
	Mcp23017_WriteRegister(&lcd_adapter, MCP23X17_SEQ_IODIRB, 0x00);

	Hd44780_Init(&display);
	Hd44780_SetCursorPos(&display, 0, 0);
	Hd44780_PutString(&display, "Hello");
	Hd44780_SetCursorPos(&display, 1, 0);
	Hd44780_PutString(&display, "world!!!");
	I2c_Close(bus);
	return 0;
}

