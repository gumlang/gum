#include "lexer.h"
#include <gum/map.h>
#include <string.h>
#include <ctype.h>

#define TOKEN_ADD_KW(name) \
	*(unsigned short*)gum_map_add(&g_keywords, -1, #name) = GUM_TOKEN_KW_##name

static gum_map_t g_keywords;

static gum_token_t g_token;
static _Bool g_peeked;

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

static _Bool lexer_next_char() {
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

gum_token_t gum_lexer_peek() {
	if (!g_peeked) {
		for (;;) {
			gum_char_t c;
			while (isspace((c = gum_input_peek()).c)) {
				gum_input_next();
			}

			if (c.c == -1) {
				g_token.type = GUM_TOKEN_EOF;
				break;
			} else if (strchr("(){}<>[].,=+-*/%~|&^!#?", c.c) != NULL) {
				if (lexer_next_char()) {
					break;
				}
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