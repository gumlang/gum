#ifndef GUM_INPUT_H_
#define GUM_INPUT_H_
#include <gum.h>
#include <gum/array.h>
#include <stdio.h>

typedef struct gum_char_t {
	gum_int_t x;
	gum_int_t y;
	short c;
} gum_char_t;

typedef struct gum_input_t {
	const char* path;
	FILE* file;
	gum_array_t lines;
	gum_char_t c;
	_Bool peeked;
} gum_input_t;

void gum_input_create(gum_input_t* input, const char* path);
void gum_input_destroy(gum_input_t* input);
gum_char_t gum_input_peek(gum_input_t* input);
gum_char_t gum_input_next(gum_input_t* input);
void gum_input_error(gum_input_t* input, gum_char_t pos, const char* format, ...);

#endif