#ifndef GUM_LEXER_H_
#define GUM_LEXER_H_
#include "input.h"
#include <gum.h>
#include <gum/string.h>
#include <gum/map.h>

#define GUM_TOKEN_FLAG_DOUBLE 0x0080
#define GUM_TOKEN_FLAG_ASSIGN 0x0100

typedef enum gum_token_type_t {
	GUM_TOKEN_EOF = 0x0200,
	GUM_TOKEN_NAME,
	GUM_TOKEN_INT,
	GUM_TOKEN_FLOAT,
	GUM_TOKEN_STRING,

	GUM_TOKEN_KW_import,
	GUM_TOKEN_KW_static,
	GUM_TOKEN_KW_native,
	GUM_TOKEN_KW_set,
	GUM_TOKEN_KW_get,
	GUM_TOKEN_KW_as,
	GUM_TOKEN_KW_is,
	GUM_TOKEN_KW_if,
	GUM_TOKEN_KW_else,
	GUM_TOKEN_KW_while,
	GUM_TOKEN_KW_for,
	GUM_TOKEN_KW_in,
	GUM_TOKEN_KW_continue,
	GUM_TOKEN_KW_break,
	GUM_TOKEN_KW_return,
	GUM_TOKEN_KW_true,
	GUM_TOKEN_KW_false,
} gum_token_type_t;

typedef struct gum_token_t {
	gum_char_t pos;
	gum_int_t type;
	union {
		gum_int_t i;
		gum_float_t f;
		gum_string_t s;
	} data;
} gum_token_t;

typedef struct gum_lexer_t {
	gum_input_t input;
	gum_map_t keywords;
	gum_token_t token;
	_Bool peeked;
} gum_lexer_t;

void gum_lexer_create(gum_lexer_t* lexer, const char* path);
void gum_lexer_destroy(gum_lexer_t* lexer);
gum_token_t gum_lexer_peek(gum_lexer_t* lexer);
gum_token_t gum_lexer_next(gum_lexer_t* lexer);

#endif