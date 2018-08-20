#include "array.h"
#include <stdlib.h>
#include <string.h>

#define ARRAY_GET(array, index) ((array)->data + (index) * (array)->elem_size)

struct gum_array_t {
	char* data;
	size_t elem_size;
	gum_int_t size;
	gum_int_t capacity;
};

gum_array_t* gum_array_create(size_t elem_size) {
	gum_array_t* array = malloc(sizeof(gum_array_t));
	array->data = NULL;
	array->elem_size = elem_size;
	array->size = 0;
	array->capacity = 0;
}

void gum_array_destroy(gum_array_t* array) {
	free(array->data);
	free(array);
}

void* gum_array_data(gum_array_t* array) {
	return array->data;
}

gum_int_t gum_array_size(gum_array_t* array) {
	return array->size;
}

gum_int_t gum_array_capacity(gum_array_t* array) {
	return array->capacity;
}

void* gum_array_resize(gum_array_t* array, gum_int_t size) {
	gum_array_reserve(array, size);
	void* result = ARRAY_GET(array, array->size);
	array->size = size;
	return result;
}

void gum_array_reserve(gum_array_t* array, gum_int_t capacity) {
	if (capacity > array->capacity) {
		capacity *= 2;
		array->data = realloc(array->data, capacity);
		array->capacity = capacity;
	}
}

void* gum_array_add(gum_array_t* array, gum_int_t index) {
	if (index == -1) {
		index = array->size;
	}
	gum_array_resize(array, array->size + 1);

	gum_int_t size = 
	memmove(ARRAY_GET(array, index), ARRAY_GET(array, index + 1), 1);
	return ARRAY_GET(array, index);
}