#include "ast.h"
#include "lexer.h"
#include "util.h"
#include <stdlib.h>
#include <string.h>

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

static gum_node_t* ast_node_create(gum_node_type_t type) {
	gum_node_t* node = malloc(sizeof(gum_node_t));
	node->type = type;
	node->child.a = NULL;
	node->child.b = NULL;
	node->child.c = NULL;
	node->data.s.data = NULL;
	node->data.a.data = NULL;
	return node;
}

static gum_node_t* ast_parse_expr(gum_lexer_t* lexer);

static gum_node_t* ast_parse_value(gum_lexer_t* lexer) {
	gum_token_t token = gum_lexer_next(lexer);
	gum_node_t* node;
	switch (token.type) {
		case '+':
		case '-':
		case '~':
		case '!':
			node = ast_node_create(GUM_NODE_UNARY);
			node->child.a = ast_parse_value(lexer);
			node->data.i = token.type;
			return node;
		case '(':
			node = ast_parse_expr(lexer);
			token = gum_lexer_next(lexer);
			if (token.type != ')') {
				ast_error(lexer, token, "symbol )");
			}
			break;
		case GUM_TOKEN_NAME:
			node = ast_node_create(GUM_NODE_NAME);
			node->data.s = token.data.s;
			break;
		case GUM_TOKEN_INT:
			node = ast_node_create(GUM_NODE_INT);
			node->data.i = token.data.i;
			return node;
		case GUM_TOKEN_FLOAT:
			node = ast_node_create(GUM_NODE_FLOAT);
			node->data.f = token.data.f;
			return node;
		case GUM_TOKEN_STRING:
			node = ast_node_create(GUM_NODE_STRING);
			node->data.s = token.data.s;
			break;
		case GUM_TOKEN_KW_false:
		case GUM_TOKEN_KW_true:
			node = ast_node_create(GUM_NODE_BOOL);
			node->data.b = token.type == GUM_TOKEN_KW_true;
			return node;
		default:
			ast_error(lexer, token, "value");
			break;
	}

	while (gum_lexer_peek(lexer).type == '.') {
		gum_node_t* next_node = ast_node_create(GUM_NODE_BINARY);
		next_node->data.i = gum_lexer_next(lexer).type;
		next_node->child.a = node;

		token = gum_lexer_next(lexer);
		if (token.type == GUM_TOKEN_NAME) {
			next_node->child.b = ast_node_create(GUM_NODE_NAME);
			next_node->child.b->data.s = token.data.s;
		}
		node = next_node;
	}
	return node;
}

static gum_int_t ast_precedence(gum_int_t type) {
	switch (type) {
		case '*':
		case '/':
		case '%':
			return 12;
		case '+':
		case '-':
			return 11;
		case '<' | GUM_TOKEN_FLAG_DOUBLE:
		case '>' | GUM_TOKEN_FLAG_DOUBLE:
			return 10;
		case '&':
			return 9;
		case '^':
			return 8;
		case '|':
			return 7;
		case '=' | GUM_TOKEN_FLAG_DOUBLE:
		case '!' | GUM_TOKEN_FLAG_ASSIGN:
		case '<' | GUM_TOKEN_FLAG_ASSIGN:
		case '>' | GUM_TOKEN_FLAG_ASSIGN:
		case '<':
		case '>':
			return 6;
		case '&' | GUM_TOKEN_FLAG_DOUBLE:
			return 5;
		case '|' | GUM_TOKEN_FLAG_DOUBLE:
			return 4;
		case GUM_TOKEN_KW_as:
		case GUM_TOKEN_KW_is:
			return 3;
		case ',':
			return 2;
		default:
			if (type == '=' || type & GUM_TOKEN_FLAG_ASSIGN) {
				return 1;
			} else {
				return -1;
			}
	}
}

static gum_node_t* ast_parse_binary(gum_lexer_t* lexer, gum_int_t prec) {
	gum_node_t* node = ast_parse_value(lexer);
	gum_int_t next_prec;
	while ((next_prec = ast_precedence(gum_lexer_peek(lexer).type)) > prec) {
		gum_int_t type = gum_lexer_next(lexer).type;
		gum_node_t* next_node = ast_node_create(GUM_NODE_BINARY);
		next_node->data.i = type;
		next_node->child.a = node;

		if (next_prec == 1) {
			next_node->type = GUM_NODE_ASSIGN;
			next_node->child.b = ast_parse_expr(lexer);
		} else {
			next_node->child.b = ast_parse_binary(lexer, next_prec);
		}
		node = next_node;
	}
	return node;
}

static inline gum_node_t* ast_parse_expr(gum_lexer_t* lexer) {
	return ast_parse_binary(lexer, 0);
}

gum_node_t* gum_ast_parse(const char* path) {
	gum_lexer_t lexer;
	gum_lexer_create(&lexer, path);
	return ast_parse_expr(&lexer);
}

void gum_ast_destroy(gum_node_t* node) {
	if (node != NULL) {
		gum_ast_destroy(node->child.a);
		gum_ast_destroy(node->child.b);
		gum_ast_destroy(node->child.c);
		gum_string_destroy(&node->data.s);
		gum_array_destroy(&node->data.a);
		free(node);
	}
}