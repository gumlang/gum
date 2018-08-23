#ifndef GUM_LEXER_H_
#define GUM_LEXER_H_
#include "input.h"
#include <gum.h>
#include <gum/string.h>

#define GUM_TOKEN_FLAG_DOUBLE 0x0080
#define GUM_TOKEN_FLAG_ASSIGN 0x0100

#define GUM_TOKEN_KW_import 0x0200
#define GUM_TOKEN_KW_static 0x0400
#define GUM_TOKEN_KW_native 0x0600
#define GUM_TOKEN_KW_set 0x0800
#define GUM_TOKEN_KW_get 0x0A00
#define GUM_TOKEN_KW_as 0x0C00
#define GUM_TOKEN_KW_if 0x0E00
#define GUM_TOKEN_KW_else 0x1000
#define GUM_TOKEN_KW_for 0x1200
#define GUM_TOKEN_KW_continue 0x1400
#define GUM_TOKEN_KW_break 0x1600
#define GUM_TOKEN_KW_return 0x1800

#define GUM_TOKEN_KW_any 0x1A00
#define GUM_TOKEN_KW_type 0x1C00
#define GUM_TOKEN_KW_int 0x1E00
#define GUM_TOKEN_KW_float 0x2000
#define GUM_TOKEN_KW_bool 0x2200
#define GUM_TOKEN_KW_true 0x2400
#define GUM_TOKEN_KW_false 0x2600

#define GUM_TOKEN_EOF 0x8000
#define GUM_TOKEN_NAME 0x8200
#define GUM_TOKEN_INT 0x8400
#define GUM_TOKEN_FLOAT 0x8600
#define GUM_TOKEN_STRING 0x8800

typedef struct gum_token_t {
	gum_char_t pos;
	unsigned short type;
	union {
		gum_int_t i;
		gum_float_t f;
		gum_string_t s;
	} data;
} gum_token_t;



void gum_lexer_init(const char* path);
gum_token_t gum_lexer_peek();
gum_token_t gum_lexer_next();

#endif