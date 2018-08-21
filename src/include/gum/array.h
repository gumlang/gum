#ifndef GUM_ARRAY_H_
#define GUM_ARRAY_H_
#include "../gum.h"

typedef struct gum_array_t {
	void* data;
	gum_int_t value_size;
	gum_int_t size;
	gum_int_t capacity;
} gum_array_t;

GUM_API void gum_array_create(gum_array_t* array, gum_int_t value_size);
GUM_API void gum_array_destroy(gum_array_t* array);
GUM_API void* gum_array_add(gum_array_t* array, gum_int_t index, gum_int_t size);
GUM_API void gum_array_remove(gum_array_t* array, gum_int_t index, gum_int_t size);
GUM_API void* gum_array_get(gum_array_t* array, gum_int_t index);

#endif