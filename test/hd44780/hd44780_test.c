#include "bus/i2c.h"
#include "gpio/mcp23x17.h"
#include "gpio/mcp23017.h"
#include "display/hd44780.h"
#include <unistd.h>
#include <stdio.h>
#include "bits.h"

I2cDevice lcd_adapter = { .address = 0x27 };

void LcdBacklight(uint8_t value)
{
	if (value) {
		Mcp23017_SetPinB(&lcd_adapter, BIT7);
	} else {
		Mcp23017_ResetPinB(&lcd_adapter, BIT7);
	}
}

void LcdWrite(uint8_t data, uint8_t flags)
{
	uint8_t device_flags;
	Mcp23017_WritePortA(&lcd_adapter, data);
	device_flags = 0;
	if (flags & HD44780_FLAG_E)
		device_flags |= BIT4;
	if (flags & HD44780_FLAG_WR)
		device_flags |= BIT5;
	if (flags & HD44780_FLAG_RS)
		device_flags |= BIT6;		
	Mcp23017_WritePortB(&lcd_adapter, device_flags );
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

	Hd44780_Init(2, 16, HD44780_DATA_MODE_8, &LcdWrite);
	Hd44780_Clear();
	Hd44780_SetCursorPos(0, 0);
	Hd44780_PutString("Hello");
	Hd44780_SetCursorPos(1, 0);
	Hd44780_PutString("world!!!");
	I2c_Close(bus);
	return 0;
}

