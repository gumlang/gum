#ifndef GUM_ARRAY_H_
#define GUM_ARRAY_H_
#include "../gum.h"

typedef struct gum_array_t gum_array_t;

GUM_API gum_array_t* gum_array_create(size_t value_size);
GUM_API void gum_array_destroy(gum_array_t* array);

GUM_API void* gum_array_data(gum_array_t* array);
GUM_API size_t gum_array_size(gum_array_t* array);
GUM_API size_t gum_array_capacity(gum_array_t* array);
GUM_API void gum_array_reserve(gum_array_t* array, size_t capacity);

GUM_API void* gum_array_add(gum_array_t* array, size_t index, size_t size);
GUM_API void gum_array_remove(gum_array_t* array, size_t index, size_t size);
GUM_API void* gum_array_get(gum_array_t* array, size_t index);

#endif