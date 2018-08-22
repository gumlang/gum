#include <gum/map.h>
#include <gum/string.h>
#include <string.h>

typedef struct map_entry_t {
	_Bool used;
	gum_string_t key;
	gum_hash_t hash;
} map_entry_t;

static map_entry_t* map_get(gum_map_t* map, gum_int_t size, const char* key, _Bool add) {
	for (;;) {
		if (size == -1) {
			size = strlen(key);
		}

		gum_hash_t hash = 16777619;
		for (int i = 0; i < size; ++i) {
			hash ^= key[i];
			hash *= 2166136261;
		}

		for (gum_int_t i = 0; i < 8; ++i) {
			map_entry_t* entry = gum_array_get(&map->array, (hash + i * i) & map->mask);
			if (entry->used) {
				if (entry->hash == hash && entry->key.size == size) {
					if (memcmp(entry->key.data, key, size) == 0) {
						return entry;
					}
				}
			} else if (add) {
				entry->hash = hash;
				return entry;
			}
		}
		
		if (add) {
			gum_int_t old_size = map->array.size;
			gum_array_add(&map->array, -1, old_size);
			gum_hash_t mask = map->mask + 1;
			for (gum_int_t i = 0; i < old_size; ++i) {
				map_entry_t* old_entry = gum_array_get(&map->array, i);
				map_entry_t* entry = gum_array_get(&map->array, i + old_size);
				if (entry->hash & mask) {
					memcpy(entry, old_entry, map->array.value_size);
					old_entry->used = 0;
				} else {
					entry->used = 0;
				}
			}
			map->mask += mask;
		} else {
			return NULL;
		}
	}
}

GUM_API void gum_map_create(gum_map_t* map, gum_int_t value_size) {
	gum_array_create(&map->array, value_size + sizeof(map_entry_t));
	gum_array_add(&map->array, 0, 16);
	for (gum_int_t i = 0; i < map->array.size; ++i) {
		map_entry_t* entry = gum_array_get(&map->array, i);
		entry->used = 0;
	}
	
	map->size = 0;
	map->mask = 0xF;
}

GUM_API void gum_map_destroy(gum_map_t* map) {
	for (gum_int_t i = 0; i < map->array.size; ++i) {
		map_entry_t* entry = gum_array_get(&map->array, i);
		if (entry->used) {
			gum_string_destroy(&entry->key);
		}
	}
	gum_array_destroy(&map->array);
}

GUM_API void* gum_map_add(gum_map_t* map, gum_int_t size, const char* key) {
	map_entry_t* entry = map_get(map, size, key, 1);
	if (!entry->used) {
		entry->used = 1;
		gum_string_create(&entry->key);
		gum_string_add(&entry->key, 0, size, key);
		++map->size;
	}
	return entry + 1;
}

GUM_API void gum_map_remove(gum_map_t* map, gum_int_t size, const char* key) {
	map_entry_t* entry = map_get(map, size, key, 0);
	if (entry != NULL) {
		entry->used = 0;
		gum_string_destroy(&entry->key);
		--map->size;
	}
}

GUM_API void* gum_map_get(gum_map_t* map, gum_int_t size, const char* key) {
	map_entry_t* entry = map_get(map, size, key, 0);
	if (entry == NULL) {
		return NULL;
	} else {
		return entry + 1;
	}
}

GUM_API void gum_mapiter_create(gum_mapiter_t* iter) {
	iter->index = 0;
}

GUM_API _Bool gum_mapiter_next(gum_map_t* map, gum_mapiter_t* iter) {
	while (iter->index < map->array.size) {
		map_entry_t* entry = gum_array_get(&map->array, iter->index++);
		if (entry->used) {
			iter->key = &entry->key;
			iter->value = entry + 1;
			return 1;
		}
	}
	return 0;
}