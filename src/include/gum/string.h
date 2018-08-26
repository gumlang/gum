#ifndef GUM_STRING_H_
#define GUM_STRING_H_
#include "../gum.h"

typedef struct gum_string_t {
	char* data;
	gum_int_t value_size;
	gum_int_t size;
	gum_int_t capacity;
} gum_string_t;

GUM_API void gum_string_create(gum_string_t* string);
GUM_API void gum_string_destroy(gum_string_t* string);
GUM_API void gum_string_add(gum_string_t* string, gum_int_t index, gum_int_t size, const char* value);
GUM_API char* gum_string_add_empty(gum_string_t* string, gum_int_t index, gum_int_t size);
GUM_API void gum_string_remove(gum_string_t* string, gum_int_t index, gum_int_t size);
GUM_API char gum_string_get(gum_string_t* string, gum_int_t index);
GUM_API char* gum_string_cstr(gum_string_t* string);

#endif