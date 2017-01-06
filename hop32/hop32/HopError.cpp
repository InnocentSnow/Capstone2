#include <stdio.h>
#include <stdarg.h>

void _cdecl hopError(const char *format, ...)
{
	va_list argList;

	va_start(argList, format);
	_vfprintf_l(stderr, format, NULL, argList);
	va_end(argList);
}
