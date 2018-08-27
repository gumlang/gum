#include "lexer.h"
#include "util.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static void lexer_error(gum_lexer_t* lexer, gum_char_t c, const char* expected) {
	const char* got;
	if (c.c == -1) {
		got = "end of file";
	} else if (c.c == '\n') {
		got = "new line";
	} else if (c.c == '\t') {
		got = "tab";
	} else if (c.c == '\'') {
		got = "single quote";
	} else if (c.c == '"') {
		got = "double quote";
	} else {
		got = gum_alloc_printf("character '%c'", c.c);
	}

	gum_input_error(&lexer->input, c, "Expected %s, got %s", expected, got);
}

static inline gum_char_t input_peek(gum_lexer_t* lexer) {
	return gum_input_peek(&lexer->input);
}

static inline gum_char_t input_next(gum_lexer_t* lexer) {
	return gum_input_next(&lexer->input);
}

static gum_int_t input_next_hex(gum_lexer_t* lexer) {
	gum_char_t c = input_next(lexer);
	if (isdigit(c.c)) {
		return c.c - '0';
	} else if (c.c >= 'A' && c.c <= 'F') {
		return c.c - 'A' + 10;
	} else if (c.c >= 'a' && c.c <= 'f') {
		return c.c - 'a' + 10;
	} else {
		lexer_error(lexer, c, "hexadecimal digit");
		return 0;
	}
}

static gum_int_t input_next_char(gum_lexer_t* lexer) {
	gum_char_t c = input_next(lexer);
	if (c.c == -1) {
		lexer_error(lexer, c, "character");
	} else if (c.c == '\\') {
		c = input_next(lexer);
		if (c.c == -1) {
			lexer_error(lexer, c, "escape character");
		} else if (c.c == 't') {
			return '\t';
		} else if (c.c == 'n') {
			return '\n';
		} else if (c.c == 'r') {
			return '\r';
		} else if (c.c == 'x' || c.c == 'X') {
			return (input_next_hex(lexer) << 4) | input_next_hex(lexer);
		} else {
			return c.c;
		}
	}
	return c.c;
}

static gum_int_t lexer_next_symbol(gum_lexer_t* lexer) {
	gum_char_t c = input_next(lexer);
	gum_int_t symbol = c.c;

	if (c.c == '/') {
		c = input_peek(lexer);
		if (c.c == '/') {
			while (input_next(lexer).c != '\n');
			return -1;
		}
		if (c.c == '*') {
			for (;;) {
				if (input_next(lexer).c == '*') {
					if (input_next(lexer).c == '/') {
						break;
					}
				}
			}
			return -1;
		}
	}

	if (strchr("<>=|&", c.c) != NULL && input_peek(lexer).c == c.c) {
		input_next(lexer);
		symbol |= GUM_TOKEN_FLAG_DOUBLE;
	}
	if (strchr("<>+-*/%|&^!", c.c) != NULL && input_peek(lexer).c == '=') {
		input_next(lexer);
		symbol |= GUM_TOKEN_FLAG_ASSIGN;
	}
	return symbol;
}

static gum_token_t lexer_next_name(gum_lexer_t* lexer) {
	gum_char_t c = input_peek(lexer);
	gum_token_t token;
	token.pos = c;
	token.type = GUM_TOKEN_NAME;
	gum_string_create(&token.data.s);

	while (isalnum((c = input_peek(lexer)).c) || c.c == '_') {
		*gum_string_add_empty(&token.data.s, -1, 1) = input_next(lexer).c;
	}

	gum_int_t* kw = gum_map_get(&lexer->keywords, token.data.s.size, token.data.s.data);
	if (kw != NULL) {
		token.type = *kw;
		gum_string_destroy(&token.data.s);
	}
	return token;
}

static gum_token_t lexer_next_number(gum_lexer_t* lexer) {
	gum_char_t c = input_peek(lexer);
	gum_token_t token;
	token.pos = c;
	token.type = GUM_TOKEN_INT;
	token.data.i = 0;

	if (c.c == '0') {
		input_next(lexer);
		c = input_peek(lexer);
		if (c.c == 'x' || c.c == 'X') {
			input_next(lexer);
			while (isxdigit(input_peek(lexer).c)) {
				token.data.i = (token.data.i << 4) | input_next_hex(lexer);
			}
			return token;
		}
	}

	gum_int_t div = 0;
	while (isdigit((c = input_peek(lexer)).c) || c.c == '.') {
		input_next(lexer);
		if (c.c == '.') {
			if (div > 0) {
				lexer_error(lexer, c, "digit");
			} else {
				div = 1;
			}
		} else {
			token.data.i = token.data.i * 10 + (c.c - '0');
			div *= 10;
		}
	}

	if (div > 0) {
		token.type = GUM_TOKEN_FLOAT;
		token.data.f = (gum_float_t)token.data.i / div;
	}
	return token;
}

static gum_token_t lexer_next_char_token(gum_lexer_t* lexer) {
	gum_char_t c = input_next(lexer);
	gum_token_t token;
	token.pos = c;
	token.type = GUM_TOKEN_INT;

	c = input_peek(lexer);
	if (c.c == '\'') {
		lexer_error(lexer, c, "character");
	} else {
		token.data.i = input_next_char(lexer);
	}
	
	c = input_next(lexer);
	if (c.c != '\'') {
		lexer_error(lexer, c, "single quote");
	}
	return token;
}

static gum_token_t lexer_next_string(gum_lexer_t* lexer) {
	gum_char_t c = input_next(lexer);
	gum_token_t token;
	token.pos = c;
	token.type = GUM_TOKEN_STRING;
	gum_string_create(&token.data.s);

	while (input_peek(lexer).c != '"') {
		*gum_string_add_empty(&token.data.s, -1, 1) = input_next_char(lexer);
	}
	input_next(lexer);
	return token;
}

void gum_lexer_create(gum_lexer_t* lexer, const char* path) {
	gum_input_create(&lexer->input, path);
	gum_map_create(&lexer->keywords, sizeof(gum_int_t));
#	define GUM_KEYWORD_LEXER_MAP(kw) \
		*(gum_int_t*)gum_map_add(&lexer->keywords, -1, #kw) = GUM_TOKEN_KW_##kw;
	GUM_KEYWORDS(GUM_KEYWORD_LEXER_MAP)
}

void gum_lexer_destroy(gum_lexer_t* lexer) {
	gum_map_destroy(&lexer->keywords);
	gum_input_destroy(&lexer->input);
}

gum_token_t gum_lexer_peek(gum_lexer_t* lexer) {
	if (!lexer->peeked) {
		for (;;) {
			gum_char_t c;
			while (isspace((c = input_peek(lexer)).c)) {
				input_next(lexer);
			}

			if (c.c == -1) {
				lexer->token.pos = c;
				lexer->token.type = GUM_TOKEN_EOF;
				break;
			} else if (strchr("(){}<>[].,=+-*/%~|&^!?:", c.c) != NULL) {
				gum_int_t symbol;
				if ((symbol = lexer_next_symbol(lexer)) != -1) {
					lexer->token.pos = c;
					lexer->token.type = symbol;
					break;
				}
			} else if (isalpha(c.c) || c.c == '_') {
				lexer->token = lexer_next_name(lexer);
				break;
			} else if (isdigit(c.c)) {
				lexer->token = lexer_next_number(lexer);
				break;
			} else if (c.c == '\'') {
				lexer->token = lexer_next_char_token(lexer);
				break;
			} else if (c.c == '"') {
				lexer->token = lexer_next_string(lexer);
				break;
			} else {
				gum_input_error(&lexer->input, c, "Unknown character '%c'", c.c);
			}
		}
		lexer->peeked = 1;
	}
	return lexer->token;
}

gum_token_t gum_lexer_next(gum_lexer_t* lexer) {
	gum_lexer_peek(lexer);
	lexer->peeked = 0;
	return lexer->token;
}