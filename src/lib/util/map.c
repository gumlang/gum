#include <gum/map.h>
#include <gum/string.h>
#include <string.h>

typedef struct map_entry_t {
	_Bool used;
	gum_string_t key;
	gum_hash_t hash;
} map_entry_t;

static void* map_get(gum_map_t* map, gum_int_t size, const char* key, _Bool add) {
	for (;;) {
		gum_hash_t hash = 16777619;
		for (int i = 0; i < size; ++i) {
			hash ^= key[i];
			hash *= 2166136261;
		}

		for (gum_int_t i = 0; i < map->array.size / 2; ++i) {
			map_entry_t* entry = gum_array_get(&map->array, (hash + i * i) & map->mask);
			if (entry->used) {
				if (entry->hash == hash && entry->key.size == size) {
					if (memcmp(entry->key.data, key, size) == 0) {
						return entry + 1;
					}
				}
			} else if (add) {
				return entry + 1;
			}
		}
		
		gum_array_add(&map->array, -1, map->array.size);
		gum_hash_t mask = map->mask + 1;
		// TODO: rehash
	}
}

GUM_API void gum_map_create(gum_map_t* map, gum_int_t value_size) {
	gum_array_create(&map->array, value_size + sizeof(map_entry_t));
	gum_array_add(&map->array, 0, 16);
	for (gum_int_t i = 0; i < map->array.size; ++i) {
		map_entry_t* entry = gum_array_get(&map->array, i);
		entry->used = 0;
	}

	map->value_size = value_size;
	map->size = 0;
	map->mask = 0xF;
}

GUM_API void gum_map_destroy(gum_map_t* map) {
	gum_array_destroy(&map->array);
}

GUM_API void* gum_map_add(gum_map_t* map, gum_int_t size, const char* key) {

}