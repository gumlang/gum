#include "input.h"
#include "error.h"
#include <gum/array.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>

static FILE* g_file;
static gum_char_t g_char;
static _Bool g_peeked;
static gum_array_t g_lines;

static void input_exit() {
	gum_array_destroy(&g_lines);
	fclose(g_file);
}

void gum_input_init(const char* path) {
	g_file = fopen(path, "rb");
	if (g_file == NULL) {
		gum_error("Failed to open '%s': %s", path, strerror(errno));
	}

	gum_array_create(&g_lines, sizeof(fpos_t));
	gum_array_add(&g_lines, -1, 1);
	atexit(input_exit);
	g_char.c = '\n';
}

gum_char_t gum_input_peek() {
	if (!g_peeked) {
		if (g_char.c == '\n') {
			g_char.x = 0;
			++g_char.y;
			fgetpos(g_file, gum_array_add(&g_lines, -1, 1));
		} else {
			++g_char.x;
		}

		while ((g_char.c = fgetc(g_file)) == '\r');
		g_peeked = 1;
	}
	return g_char;
}

gum_char_t gum_input_next() {
	gum_input_peek();
	g_peeked = 0;
	return g_char;
}

void gum_input_error(gum_char_t pos, const char* format, ...) {
	pos.x += fprintf(stderr, "%i: ", pos.y);

	fsetpos(g_file, gum_array_get(&g_lines, pos.y));
	gum_char_t c;
	while ((c = gum_input_next()).c != '\n' && c.c != -1) {
		fputc(c.c, stderr);
	}
	fputc('\n', stderr);

	for (gum_int_t i = 0; i < pos.x; ++i) {
		fputc(' ', stderr);
	}
	fputs("^ ", stderr);
	
	va_list list;
	va_start(list, format);
	vfprintf(stderr, format, list);
	fputc('\n', stderr);
	va_end(list);
	exit(EXIT_FAILURE);
}