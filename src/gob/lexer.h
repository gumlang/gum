#ifndef GUM_LEXER_H_
#define GUM_LEXER_H_
#include "input.h"
#include "keywords.h"
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
#	define GUM_KEYWORD_TOKEN_TYPE(kw) \
		GUM_TOKEN_KW_##kw,
	GUM_KEYWORDS(GUM_KEYWORD_TOKEN_TYPE)
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