#ifndef GUM_INPUT_H_
#define GUM_INPUT_H_
#include <gum.h>

typedef struct gum_char_t {
	gum_int_t x;
	gum_int_t y;
	short c;
} gum_char_t;

void gum_input_init(const char* path);
gum_char_t gum_input_peek();
gum_char_t gum_input_next();
void gum_input_error(gum_char_t pos, const char* format, ...);

#endif