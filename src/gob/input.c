#include "input.h"
#include "error.h"
#include <gum/string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>

void gum_input_create(gum_input_t* input, const char* path) {
	input->path = path;
	input->file = fopen(path, "rb");
	if (input->file == NULL) {
		gum_error("Failed to open '%s': %s", path, strerror(errno));
	}

	gum_array_create(&input->lines, sizeof(fpos_t));
	gum_array_add(&input->lines, -1, 1);
	input->c.y = 0;
	input->c.c = '\n';
	input->peeked = 0;
}

void gum_input_destroy(gum_input_t* input) {
	gum_array_destroy(&input->lines);
	fclose(input->file);
}

gum_char_t gum_input_peek(gum_input_t* input) {
	if (!input->peeked) {
		if (input->c.c == '\n') {
			input->c.x = 0;
			++input->c.y;
			fgetpos(input->file, gum_array_add(&input->lines, -1, 1));
		} else {
			++input->c.x;
		}

		while ((input->c.c = fgetc(input->file)) == '\r');
		input->peeked = 1;
	}
	return input->c;
}

gum_char_t gum_input_next(gum_input_t* input) {
	gum_input_peek(input);
	input->peeked = 0;
	return input->c;
}

void gum_input_error(gum_input_t* input, gum_char_t pos, const char* format, ...) {
	pos.x += fprintf(stderr, "%s:%i: ", input->path, pos.y);

	fsetpos(input->file, gum_array_get(&input->lines, pos.y));
	input->peeked = 0;
	gum_char_t c;
	while ((c = gum_input_next(input)).c != '\n' && c.c != -1) {
		if (c.c == '\t') {
			c.c = ' ';
		}
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