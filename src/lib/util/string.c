#include <gum/string.h>
#include <gum/array.h>
#include <string.h>

GUM_API void gum_string_create(gum_string_t* string) {
	gum_array_create((gum_array_t*)string, 1);
}

GUM_API void gum_string_destroy(gum_string_t* string) {
	gum_array_destroy((gum_array_t*)string);
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