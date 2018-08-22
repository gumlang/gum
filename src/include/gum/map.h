#ifndef GUM_MAP_H_
#define GUM_MAP_H_
#include "../gum.h"
#include "array.h"
#include "string.h"

typedef uint32_t gum_hash_t;

typedef struct gum_mapiter_t {
	gum_int_t index;
	gum_string_t* key;
	void* value;
} gum_mapiter_t;

typedef struct gum_map_t {
	gum_array_t array;
	gum_int_t size;
	gum_hash_t mask;
} gum_map_t;

GUM_API void gum_map_create(gum_map_t* map, gum_int_t value_size);
GUM_API void gum_map_destroy(gum_map_t* map);
GUM_API void* gum_map_add(gum_map_t* map, gum_int_t size, const char* key);
GUM_API void gum_map_remove(gum_map_t* map, gum_int_t size, const char* key);
GUM_API void* gum_map_get(gum_map_t* map, gum_int_t size, const char* key);

GUM_API void gum_mapiter_create(gum_mapiter_t* iter);
GUM_API _Bool gum_mapiter_next(gum_map_t* map, gum_mapiter_t* iter);

#endif