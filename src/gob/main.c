#include "ast.h"
#include <stdlib.h>
#include <stdio.h>

#ifdef GUM_DEBUG
	static void freeze() {
		puts("\nPress enter to continue");
		getchar();
	}
#endif

int main() {
#	ifdef GUM_DEBUG
		atexit(freeze);
#	endif

	gum_node_t* node = gum_ast_parse("src/Main.gum");
	gum_ast_destroy(node);
}