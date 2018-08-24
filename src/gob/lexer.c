#include "lexer.h"
#include <gum/map.h>
#include <string.h>
#include <ctype.h>


#define TOKEN_ADD_KW(name) \
	*(unsigned short*)gum_map_add(&g_keywords, -1, #name) = GUM_TOKEN_KW_##name

static gum_map_t g_keywords;

static gum_token_t g_token;
static _Bool g_peeked;

static void lexer_error_expected(const char* expected, gum_char_t got) {
	if (got.c == -1) {
		gum_input_error(got, "Expected %s, got end of file", expected);
	} else if (got.c == '\n') {
		gum_input_error(got, "Expected %s, got new line", expected);
	} else if (got.c == '\t') {
		gum_input_error(got, "Expected %s, got tab", expected);
	} else {
		gum_input_error(got, "Expected %s, got character '%c'", expected, got.c);
	}
}

static gum_int_t lexer_convert_hex(gum_char_t c) {
	if (isdigit(c.c)) {
		return c.c - '0';
	} else if (c.c >= 'A' && c.c <= 'F') {
		return c.c - 'A' + 10;
	} else if (c.c >= 'a' && c.c <= 'f') {
		return c.c - 'a' + 10;
	} else {
		lexer_error_expected("hexadecimal digit", c);
		return 0;
	}
}

static gum_int_t lexer_convert_char(gum_char_t c) {
	if (c.c == -1) {
		lexer_error_expected("character", c);
	} else if (c.c == '\\') {
		c = gum_input_next();
		if (c.c == -1) {
			lexer_error_expected("escape character", c);
		} else if (c.c == 't') {
			return '\t';
		} else if (c.c == 'n') {
			return '\n';
		} else if (c.c == 'r') {
			return '\r';
		} else if (c.c == 'x' || c.c == 'X') {
			return (lexer_convert_hex(gum_input_next()) << 4) | lexer_convert_hex(gum_input_next());
		} else {
			return c.c;
		}
	}
	return c.c;
}

static _Bool lexer_symbol() {
	gum_char_t c = gum_input_next();
	g_token.pos = c;
	g_token.type = c.c;

	if (strchr("<>=|&", c.c) != NULL && gum_input_peek().c == c.c) {
		gum_input_next();
		g_token.type |= GUM_TOKEN_FLAG_DOUBLE;
	}

	if (strchr("<>+-*/%~|&^", c.c) != NULL && gum_input_peek().c == '=') {
		gum_input_next();
		g_token.type |= GUM_TOKEN_FLAG_ASSIGN;
	}

	if (c.c == '/') {
		c = gum_input_peek();
		if (c.c == '/') {
			while (gum_input_next().c != '\n');
			return 0;
		}
		if (c.c == '*') {
			for (;;) {
				if (gum_input_next().c == '*') {
					if (gum_input_next().c == '/') {
						break;
					}
				}
			}
			return 0;
		}
	}

	return 1;
}

static void lexer_name() {
	gum_char_t c = gum_input_peek();
	g_token.pos = c;
	g_token.type = GUM_TOKEN_NAME;
	gum_string_create(&g_token.data.s);

	while (isalnum((c = gum_input_peek()).c) || c.c == '_') {
		char value = gum_input_next().c;
		gum_string_add(&g_token.data.s, -1, 1, &value);
	}

	unsigned short* kw = gum_map_get(&g_keywords, g_token.data.s.size, g_token.data.s.data);
	if (kw != NULL) {
		g_token.type = *kw;
		gum_string_destroy(&g_token.data.s);
	}
}

static void lexer_number() {
	gum_char_t c = gum_input_peek();
	g_token.pos = c;
	g_token.type = GUM_TOKEN_INT;
	g_token.data.i = 0;

	if (c.c == '0') {
		gum_input_next();
		c = gum_input_peek();
		if (c.c == 'x' || c.c == 'X') {
			gum_input_next();
			while (isxdigit((c = gum_input_peek()).c)) {
				g_token.data.i = (g_token.data.i << 4) | lexer_convert_hex(gum_input_next());
			}
			return;
		}
	}

	gum_int_t div = 0;
	while (isdigit((c = gum_input_peek()).c) || c.c == '.') {
		gum_input_next();
		if (c.c == '.') {
			if (div > 0) {
				lexer_error_expected("digit", c);
			} else {
				div = 1;
			}
		} else {
			g_token.data.i = g_token.data.i * 10 + (c.c - '0');
			div *= 10;
		}
	}

	if (div > 0) {
		g_token.type = GUM_TOKEN_FLOAT;
		g_token.data.f = (gum_float_t)g_token.data.i / div;
	}
}

static void lexer_char() {
	gum_char_t c = gum_input_next();
	g_token.pos = c;
	g_token.type = GUM_TOKEN_INT;
	g_token.data.i = lexer_convert_char(gum_input_next());
	
	c = gum_input_next();
	if (c.c != '\'') {
		lexer_error_expected("single quote", c);
	}
}

static void lexer_string() {
	gum_char_t c = gum_input_next();
	g_token.pos = c;
	g_token.type = GUM_TOKEN_STRING;
	gum_string_create(&g_token.data.s);

	while ((c = gum_input_next()).c != '"') {
		char value = lexer_convert_char(c);
		gum_string_add(&g_token.data.s, -1, 1, &value);
	}
}

void gum_lexer_init(const char* path) {
	gum_input_init(path);

	if (g_keywords.size == 0) {
		gum_map_create(&g_keywords, sizeof(unsigned short));
		TOKEN_ADD_KW(import);
		TOKEN_ADD_KW(static);
		TOKEN_ADD_KW(native);
		TOKEN_ADD_KW(set);
		TOKEN_ADD_KW(get);
		TOKEN_ADD_KW(as);
		TOKEN_ADD_KW(if);
		TOKEN_ADD_KW(else);
		TOKEN_ADD_KW(for);
		TOKEN_ADD_KW(continue);
		TOKEN_ADD_KW(break);
		TOKEN_ADD_KW(return);

		TOKEN_ADD_KW(any);
		TOKEN_ADD_KW(type);
		TOKEN_ADD_KW(int);
		TOKEN_ADD_KW(float);
		TOKEN_ADD_KW(bool);
		TOKEN_ADD_KW(true);
		TOKEN_ADD_KW(false);
	}
}

gum_token_t gum_lexer_peek() {
	if (!g_peeked) {
		for (;;) {
			gum_char_t c;
			while (isspace((c = gum_input_peek()).c)) {
				gum_input_next();
			}

			if (c.c == -1) {
				g_token.pos = c;
				g_token.type = GUM_TOKEN_EOF;
				break;
			} else if (strchr("(){}<>[].,=+-*/%~|&^!#?", c.c) != NULL) {
				if (lexer_symbol()) {
					break;
				}
			} else if (isalpha(c.c) || c.c == '_') {
				lexer_name();
				break;
			} else if (isdigit(c.c)) {
				lexer_number();
				break;
			} else if (c.c == '\'') {
				lexer_char();
				break;
			} else if (c.c == '"') {
				lexer_string();
				break;
			} else {
				gum_input_error(c, "Unknown character '%c'", c.c);
			}
		}
		g_peeked = 1;
	}
	return g_token;
}

gum_token_t gum_lexer_next() {
	gum_lexer_peek();
	g_peeked = 0;
	return g_token;
}