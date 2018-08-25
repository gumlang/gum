#include <gum/array.h>
#include <stdlib.h>
#include <string.h>

static inline void* array_get(gum_array_t* array, gum_int_t index) {
	return array->data + index * array->value_size;
}

static void* array_resize(gum_array_t* array, gum_int_t index, gum_int_t size, gum_int_t resize) {
	if (index == -1) {
		index = array->size;
	}
	if (size == -1) {
		size = array->size - index;
	}

	gum_int_t capacity = array->size - size + resize;
	if (capacity > array->capacity) {
		if (array->capacity == 0) {
			array->capacity = 1;
		}
		while (array->capacity < capacity) {
			array->capacity *= 2;
		}
		array->data = realloc(array->data, array->capacity * array->value_size);
	}

	gum_int_t move = array->size - index - size;
	if (move > 0) {
		memmove(
			array_get(array, index + resize),
			array_get(array, index + size),
			move * array->value_size
		);
	}
	array->size = capacity;
	return array_get(array, index);
}

GUM_API void gum_array_create(gum_array_t* array, gum_int_t value_size) {
	array->data = NULL;
	array->value_size = value_size;
	array->size = 0;
	array->capacity = 0;
}

GUM_API void gum_array_destroy(gum_array_t* array) {
	free(array->data);
}

GUM_API void* gum_array_add(gum_array_t* array, gum_int_t index, gum_int_t size) {
	return array_resize(array, index, 0, size);
}

GUM_API void gum_array_remove(gum_array_t* array, gum_int_t index, gum_int_t size) {
	array_resize(array, index, size, 0);
}

GUM_API void* gum_array_get(gum_array_t* array, gum_int_t index) {
	return array_get(array, index);
}