#ifndef _TXPRINTF_H
#define _TXPRINTF_H

#include <stdarg.h>

typedef const void * putc_obj_t;
typedef void (*putc_t)(putc_obj_t obj, char c);

int txprintf(putc_t putc, putc_obj_t obj, char *format, va_list args);

int tsprintf(char *s, char *format, ...);

#endif /* _TINY_PRINTF_H */
