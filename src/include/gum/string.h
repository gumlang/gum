#ifndef GUM_STRING_H_
#define GUM_STRING_H_
#include "../gum.h"
#include <stdio.h>

typedef struct gum_string_t gum_string_t;

GUM_API gum_string_t* gum_string_create();
GUM_API void gum_string_destroy(gum_string_t* string);

GUM_API char* gum_string_data(gum_string_t* string);
GUM_API gum_int_t gum_string_size(gum_string_t* string);
GUM_API gum_int_t gum_string_capacity(gum_string_t* string);
GUM_API void gum_string_reserve(gum_string_t* string, gum_int_t capacity);

GUM_API void gum_string_add(gum_string_t* string, gum_int_t index, gum_int_t size, const char* value);
GUM_API char* gum_string_add_empty(gum_string_t* string, gum_int_t index, gum_int_t size);
GUM_API void gum_string_remove(gum_string_t* string, gum_int_t index, gum_int_t size);
GUM_API char gum_string_get(gum_string_t* string, gum_int_t index);

#endif