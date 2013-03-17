#include <delay.h>
#include "display/hd44780.h"

Hd44780WriteFunc *_write;

void (*_lcd_write)(uint8_t data, uint8_t flags);

void _lcd_write8(uint8_t data, uint8_t flags)
{
	_write(data, flags | HD44780_FLAG_E);
	delay_us(10);
	_write(data, flags);
}

void _lcd_write4(uint8_t data, uint8_t flags)
{
	uint8_t nibble;
	nibble = (data & 0xF0) | flags;
	_lcd_write8(nibble, flags);

	nibble = (data << 4) | flags;
	_lcd_write8(nibble, flags);
}

inline void _lcd_command(uint8_t command)
{
	_lcd_write(command, 0x00);
	delay_us(40);
}

inline void Hd44780_Clear(void) {
	_lcd_command(HD44780_CMD_CLEAR);
}

void Hd44780_SetCursorPos(uint8_t line, uint8_t column) {
	uint8_t address = ((line & 0x01) << 6) + ((line & 0x02) << 3) + column;
	_lcd_command(HD44780_CMD_SET_ADDRESS | address);
}

void Hd44780_PutChar(char c)
{
	_lcd_write(c, HD44780_FLAG_RS);
	delay_us(40);
}

void Hd44780_PutStringC(char *s, uint8_t count)
{
	while (count > 0) {
		Hd44780_PutChar(*s);
		s++;
		count--;
	}
}


void Hd44780_PutString(char *s)
{
	while ((s != 0) && (*s != 0))
	{
		Hd44780_PutChar(*s);
		s++;
	}
}

void Hd44780_Init(uint8_t lines, uint8_t columns, uint8_t data_length, Hd44780WriteFunc *write_func)
{
	_write = write_func;
	_lcd_write = _lcd_write8;

	delay_ms(50);
	_lcd_command(HD44780_CMD_FUNC_SET + HD44780_DATA_MODE_8);
	delay_ms(10);
	_lcd_command(HD44780_CMD_FUNC_SET + HD44780_DATA_MODE_8);
	delay_ms(1);
	_lcd_command(HD44780_CMD_FUNC_SET + data_length);
	delay_ms(100);
	if (data_length == HD44780_DATA_MODE_4) {
		_lcd_write = _lcd_write4;
	}

	_lcd_command(HD44780_CMD_FUNC_SET +
			data_length +
			HD44780_LINES_2 +
			HD44780_FONT_5x8);
	_lcd_command(HD44780_CMD_FUNC_SET +
			data_length +
			HD44780_LINES_2 +
			HD44780_FONT_5x8);
	_lcd_command(HD44780_CMD_DISPLAY_CTRL +
			HD44780_DISPLAY_ON);
	_lcd_command(HD44780_CMD_ENTRY_MODE +
			HD44780_CURSOR_INC);
	Hd44780_Clear();
	_lcd_command(HD44780_CMD_HOME);
	delay_ms(5);
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

