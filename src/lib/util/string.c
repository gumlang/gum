#include <gum/string.h>
#include <gum/array.h>
#include <string.h>

struct gum_string_t {
	char* data;
	gum_int_t value_size;
	gum_int_t size;
	gum_int_t capacity;
};

GUM_API gum_string_t* gum_string_create() {
	return (gum_string_t*)gum_array_create(1);
}

GUM_API void gum_string_destroy(gum_string_t* string) {
	gum_array_destroy((gum_array_t*)string);
}

GUM_API char* gum_string_data(gum_string_t* string) {
	return string->data;
}

GUM_API gum_int_t gum_string_size(gum_string_t* string) {
	return string->size;
}

GUM_API gum_int_t gum_string_capacity(gum_string_t* string) {
	return string->capacity;
}

GUM_API void gum_string_reserve(gum_string_t* string, gum_int_t capacity) {
	gum_array_reserve((gum_array_t*)string, capacity);
}

GUM_API void gum_string_add(gum_string_t* string, gum_int_t index, gum_int_t size, const char* value) {
	if (size == -1) {
		size = strlen(value);
	}
	
	memcpy(
		gum_string_add_empty(string, index, size),
		value, size
	);
}

GUM_API char* gum_string_add_empty(gum_string_t* string, gum_int_t index, gum_int_t size) {
	return gum_array_add((gum_array_t*)string, index, size);
}

GUM_API void gum_string_remove(gum_string_t* string, gum_int_t index, gum_int_t size) {
	gum_array_remove((gum_array_t*)string, index, size);
}

GUM_API char gum_string_get(gum_string_t* string, gum_int_t index) {
	return string->data[index];
}