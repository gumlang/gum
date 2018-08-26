#include "ast.h"
#include "lexer.h"
#include "input.h"
#include "util.h"

#define ERROR_ADD_KW(name) \
	case GUM_TOKEN_KW_##name: got = #name; break

static void ast_error(gum_lexer_t* lexer, gum_token_t token, const char* expected) {
	const char* got;
	switch (token.type) {
		case GUM_TOKEN_EOF:
			got = "end of file";
			break;
		case GUM_TOKEN_NAME:
			got = gum_alloc_printf("name %s", gum_string_cstr(&token.data.s));
			break;
		case GUM_TOKEN_INT:
			got = gum_alloc_printf("number %i", token.data.i);
			break;
		case GUM_TOKEN_FLOAT:
			got = gum_alloc_printf("number %f", token.data.f);
			break;
		case GUM_TOKEN_STRING:
			got = gum_alloc_printf("string \"%s\"", gum_string_cstr(&token.data.s));
			break;
#		define GUM_KEYWORD_ERROR_CASE(kw) \
			case GUM_TOKEN_KW_##kw: got = #kw; break;
		GUM_KEYWORDS(GUM_KEYWORD_ERROR_CASE)
		default:;
			gum_string_t symbol;
			gum_string_create(&symbol);
			*gum_string_add_empty(&symbol, -1, 1) = token.type & 0x7F;
			if (token.type & GUM_TOKEN_FLAG_DOUBLE) {
				*gum_string_add_empty(&symbol, -1, 1) = token.type & 0x7F;
			}
			if (token.type & GUM_TOKEN_FLAG_ASSIGN) {
				*gum_string_add_empty(&symbol, -1, 1) = '=';
			}
			got = gum_alloc_printf("symbol %s", gum_string_cstr(&symbol));
			gum_string_destroy(&symbol);
	}
	gum_input_error(&lexer->input, token.pos, "Expected %s, got %s", expected, got);
}

// static gum_node_t ast_parse_value(gum_lexer_t* lexer) {
// 	gum_token_t token = gum_lexer_next(lexer);
// 	if (token.type == '')
// }

gum_node_t* gum_ast_parse(const char* path) {
	gum_lexer_t lexer;
	gum_lexer_create(&lexer, path);
	return NULL;
}