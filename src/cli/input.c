#include "input.h"
#include <stdlib.h>
#include <stdio.h>

static FILE* g_file;
static short g_peek = -1;

static void input_destroy() {
	fclose(g_file);
}

void gum_input_init(const char* path) {
	g_file = fopen(path, "r");
	atexit(input_destroy);
}

short gum_input_peek() {
	if (g_peek == -1) {
		g_peek = fgetc(g_file);
	}
	return g_peek;
}

short gum_input_next() {
	short result = gum_input_peek();
	g_peek = -1;
	return result;
}