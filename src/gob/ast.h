#ifndef GUM_AST_H_
#define GUM_AST_H_
#include <gum.h>

typedef enum gum_ast_type_t {
	GUM_AST_UNARY,
	GUM_AST_BINARY,
	GUM_AST_CALL,
	GUM_AST_BLOCK,
	GUM_AST_FUNC,
	GUM_AST_IF,
	GUM_AST_WHILE,
	GUM_AST_FOR
} gum_ast_type_t;


#endif