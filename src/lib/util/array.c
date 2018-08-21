#include <gum/array.h>
#include <stdlib.h>
#include <string.h>

#define ARRAY_GET(array, index) ((array)->data + (index) * (array)->value_size)

struct gum_array_t {
	void* data;
	gum_int_t value_size;
	gum_int_t size;
	gum_int_t capacity;
};

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
		array->data = realloc(array->data, array->capacity);
	}

	gum_int_t move = array->size - index - size;
	if (move > 0) {
		memmove(
			ARRAY_GET(array, index + resize),
			ARRAY_GET(array, index + size),
			move * array->value_size
		);
	}
	array->size = capacity;
	return ARRAY_GET(array, index);
}

GUM_API gum_array_t* gum_array_create(gum_int_t value_size) {
	gum_array_t* array = malloc(sizeof(gum_array_t));
	array->data = NULL;
	array->value_size = value_size;
	array->size = 0;
	array->capacity = 0;
	return array;
}

GUM_API void gum_array_destroy(gum_array_t* array) {
	free(array->data);
	free(array);
}

GUM_API void* gum_array_data(gum_array_t* array) {
	return array->data;
}

GUM_API gum_int_t gum_array_size(gum_array_t* array) {
	return array->size;
}

GUM_API gum_int_t gum_array_capacity(gum_array_t* array) {
	return array->capacity;
}

GUM_API void gum_array_reserve(gum_array_t* array, gum_int_t capacity) {
	array->data = realloc(array->data, capacity);
	array->capacity = capacity;
	if (capacity < array->size) {
		array->size = capacity;
	}
}

GUM_API void* gum_array_add(gum_array_t* array, gum_int_t index, gum_int_t size) {
	return array_resize(array, index, 0, size);
}

GUM_API void gum_array_remove(gum_array_t* array, gum_int_t index, gum_int_t size) {
	array_resize(array, index, size, 0);
}

GUM_API void* gum_array_get(gum_array_t* array, gum_int_t index) {
	return ARRAY_GET(array, index);
}