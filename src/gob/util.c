#include "error.h"
#include <gum.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

char* gum_alloc_printf(const char* format, ...) {
	va_list list;
	va_start(list, format);
	gum_int_t size = vsnprintf(NULL, 0, format, list) + 1;
	va_end(list);
	
	const char* string = malloc(size);
	va_start(list, format);
	vsnprintf(string, size, format, list);
	va_end(list);
	return string;
}

void gum_error(const char* format, ...) {
	va_list list;
	va_start(list, format);
	vfprintf(stderr, format, list);
	fputc('\n', stderr);
	va_end(list);
	exit(EXIT_FAILURE);
}