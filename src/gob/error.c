#include "error.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

void gum_error(const char* format, ...) {
	va_list list;
	va_start(list, format);
	vfprintf(stderr, format, list);
	fputc('\n', stderr);
	va_end(list);
	exit(EXIT_FAILURE);
}