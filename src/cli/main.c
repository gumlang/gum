#include <gum/map.h>
#include <stdlib.h>

int main() {
	gum_map_t map;
	gum_map_create(&map, sizeof(int));

	for (int i = 0; i < 26; ++i) {
		char k = 'A' + i;
		int v = i + 1;
		*(int*)gum_map_add(&map, 1, &k) = v;
	}

	for (int i = 0; i < 13; ++i) {
		char k = 'A' + rand() % 26;
		gum_map_remove(&map, 1, &k);
	}

	for (int i = 0; i < 26; ++i) {
		char k = 'A' + i;
		int* v = (int*)gum_map_get(&map, 1, &k);
		if (v != NULL) {
			printf("%c\t%i\n", k, *v);
		}
	}

	printf("\n");

	gum_mapiter_t iter;
	gum_mapiter_create(&iter);
	while (gum_mapiter_next(&map, &iter)) {
		fwrite(iter.key->data, iter.key->size, 1, stdout);
		printf(
			"\t%i\t%i\n",
			*(int*)iter.value,
			*(int*)gum_map_get(&map, iter.key->size, iter.key->data)
		);
	}

	getchar();
}