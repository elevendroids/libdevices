#include <delay.h>
#include "display/hd44780.h"

static void _lcd_write8(Hd44780WriteFunc write_func, const uint8_t data, const uint8_t flags)
{
	write_func(data, flags | HD44780_FLAG_E);
	delay_us(10);
	write_func(data, flags);
}

static inline void _lcd_write4(Hd44780WriteFunc write_func, const uint8_t data, const uint8_t flags)
{
	uint8_t nibble;
	nibble = (data >> 0x04);
	_lcd_write8(write_func, nibble, flags);

	nibble = (data & 0x0F);
	_lcd_write8(write_func, nibble, flags);
}

static void Hd44780_Write(Hd44780Device *device, const uint8_t data, const uint8_t flags)
{
	const Hd44780Config *config = device->config;
	const Hd44780WriteFunc write_func = config->write_func;
	if (config->data_mode == HD44780_DATA_MODE_4)
		_lcd_write4(write_func, data, flags);
	else
		_lcd_write8(write_func, data, flags);
	delay_us(50);
}

void Hd44780_Clear(Hd44780Device *device) {
	Hd44780_Write(device, HD44780_CMD_CLEAR, 0x00);
	delay_ms(2);
}

void Hd44780_SetCursorPos(Hd44780Device *device, const uint8_t line, const uint8_t column) {
	uint8_t address = HD44780_ADDRESSES[device->config->format][line] | column;
	Hd44780_Write(device, HD44780_CMD_SET_ADDRESS | address, 0x00);
}

inline void Hd44780_PutChar(Hd44780Device *device, const char c)
{
	Hd44780_Write(device, c, HD44780_FLAG_RS);
}

void Hd44780_PutStringC(Hd44780Device *device, char *s, uint8_t count)
{
	while (count > 0) {
		Hd44780_PutChar(device, *s);
		s++;
		count--;
	}
}


void Hd44780_PutString(Hd44780Device *device, char *s)
{
	while ((s != 0) && (*s != 0))
	{
		Hd44780_PutChar(device, *s);
		s++;
	}
}

void Hd44780_DisplayOn(Hd44780Device *device)
{
	device->display_control |= HD44780_DISPLAY_ON;
	Hd44780_Write(device, HD44780_CMD_DISPLAY_CTRL | device->display_control, 0x00);
}

void Hd44780_DisplayOff(Hd44780Device *device)
{
	device->display_control &= ~HD44780_DISPLAY_ON;
	Hd44780_Write(device, HD44780_CMD_DISPLAY_CTRL | device->display_control, 0x00);
}

void Hd44780_Init(Hd44780Device *device)
{
	const Hd44780Config *config = device->config;
	device->display_control = 0x00;
	
	delay_ms(50);
	Hd44780_Write(device, HD44780_CMD_FUNC_SET | HD44780_DATA_MODE_8, 0x00);
	delay_ms(5);
	Hd44780_Write(device, HD44780_CMD_FUNC_SET | HD44780_DATA_MODE_8, 0x00);
	delay_us(200);
	Hd44780_Write(device, HD44780_CMD_FUNC_SET | config->data_mode, 0x00);
//	delay_us(100);
	Hd44780_Write(device, HD44780_CMD_FUNC_SET | config->data_mode | HD44780_LINES_2 | HD44780_FONT_5x8, 0x00);

	Hd44780_DisplayOn(device);
	Hd44780_Write(device, HD44780_CMD_ENTRY_MODE | HD44780_CURSOR_INC, 0x00);
	Hd44780_Clear(device);
	Hd44780_Write(device, HD44780_CMD_HOME, 0x00);
	delay_ms(2);
}

//void Hd44780_set_font_char(uint8_t count, uint8_t *data)
//{
//	int i;
//	_lcd_command(HD44780_CMD_SET_CG_ADDRESS);
//	for (i = 0; i < (count * 8); i++) {
//		HD44780_putc(*data);
//		data++;
//	}
//	_lcd_command(HD44780_CMD_SET_ADDRESS);
//}

