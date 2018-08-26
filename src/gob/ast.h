#ifndef GUM_AST_H_
#define GUM_AST_H_
#include "input.h"
#include <gum.h>
#include <gum/array.h>
#include <gum/string.h>

typedef enum gum_node_type_t {
	GUM_NODE_NAME,
	GUM_NODE_INT,
	GUM_NODE_FLOAT,
	GUM_NODE_STRING,
	GUM_NODE_BOOL,

	GUM_NODE_IMPORT,
	GUM_NODE_PROP,
	GUM_NODE_UNARY,
	GUM_NODE_BINARY,
	GUM_NODE_BLOCK,
	GUM_NODE_FUNC,
	GUM_NODE_IF,
	GUM_NODE_WHILE,
	GUM_NODE_FOR
} gum_node_type_t;

typedef struct gum_node_t {
	gum_node_type_t type;
	struct {
		struct gum_node_t* a;
		struct gum_node_t* b;
		struct gum_node_t* c;
	} child;
	union {
		gum_int_t i;
		gum_float_t f;
		gum_string_t s;
		_Bool b;
		gum_array_t a;
	} data;
} gum_node_t;

gum_node_t* gum_ast_parse(const char* path);
void gum_ast_destroy(gum_node_t* node);

#endif