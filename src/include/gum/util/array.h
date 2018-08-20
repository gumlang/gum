#ifndef GUM_UTIL_ARRAY_H_
#define GUM_UTIL_ARRAY_H_
#include "../types.h"

typedef struct gum_array_t gum_array_t;

gum_array_t* gum_array_create(size_t elem_size);
void gum_array_destroy(gum_array_t* array);

void* gum_array_data(gum_array_t* array);
gum_int_t gum_array_size(gum_array_t* array);
gum_int_t gum_array_capacity(gum_array_t* array);

void* gum_array_resize(gum_array_t* array, gum_int_t size);
void gum_array_reserve(gum_array_t* array, gum_int_t capacity);

void* gum_array_add(gum_array_t* array, gum_int_t index);
void gum_array_remove(gum_array_t* array, gum_int_t index);

void* gum_array_get(gum_array_t* array, gum_int_t index);

#endif