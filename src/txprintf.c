#include <stdarg.h>
#include <stdint.h>
#include <limits.h>
#include "txprintf.h"

static const int base16[] = {
#if (INT_MAX > 100000)
	0x10000000,
	0x1000000,
	0x100000,
	0x10000,
#endif
	0x1000,
	0x100,
	0x10,
	1
};

static const int base10[] = {
#if (INT_MAX > 100000)
	1000000000,
	100000000,
	10000000,
	1000000,
	100000,
#endif
	10000,
	1000,
	100,
	10,
	1
};

static int put_uint(putc_t putc, putc_obj_t obj, unsigned int v, const int *base_powers)
{
	static const char digits[] = "0123456789ABCDEF";
	int result = 0;
	int digit;

	if (v != 0) {
		do
		{
			digit = 0;
			while (v >= *base_powers)
			{
				v -= *base_powers;
				digit++;
			}

			if ((digit != 0) || (result != 0))
			{
				putc(obj, digits[digit]);
				result++;
			}
		} while (*base_powers++ != 1);
	}
	else
	{
		putc(obj, '0');
		result++;
	}
		
	return result;
}

static int put_int(putc_t putc, putc_obj_t obj, int v)
{
	int result = 0;
	unsigned int uv;
	if (v < 0)
	{
		putc(obj, '-');
		uv = -v;
		result++;
	}
	else
	{
		uv = v;
	}
	result += put_uint(putc, obj, uv, base10);
	return result;
}

static int put_string(putc_t putc, putc_obj_t obj, char *s)
{
	int result = 0;
	while (*s)
	{
		putc(obj, *s++);
		result++;
	}
	return result;
}

int txprintf(putc_t putc, putc_obj_t obj, char *format, va_list args)
{
	int result = 0;
	char c;

	while ((c = *format++))
	{
		if (c == '%')
		{
			c = *format++;
			
			switch(c)
			{
				case 'x':
						result += put_uint(putc, obj, va_arg(args, unsigned int), base16);
						break;
				case 'u':
						result += put_uint(putc, obj, va_arg(args, unsigned int), base10);
						break;
				case 'd':
						result += put_int(putc, obj, va_arg(args, int));
						break;
				case 's':
						result += put_string(putc, obj, va_arg(args, char *));
						break;
				case 'c':
						putc(obj, (char)va_arg(args, int));
						result++;
						break;
				case '%':
						putc(obj, c);
						result++;
						break;
				default:
						break;
			}
		}
		else
		{
			putc(obj, c);
			result++;
		}
	}

	return result;
}

static void tsprintf_putc(putc_obj_t obj, char c)
{
	*(*((char **)obj))++ = c;
}

int tsprintf(char *s, char *format, ...)
{
	va_list args;
	int result;
	va_start(args, format);
	result = txprintf(&tsprintf_putc, &s, format, args);
	tsprintf_putc(&s, 0);
	va_end(args);
	return result;
}

