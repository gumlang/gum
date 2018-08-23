#include "lexer.h"
#include <stdlib.h>

#ifdef GUM_DEBUG
	static void freeze() {
		puts("\nPress enter to continue");
		getchar();
	}
#endif

int main() {
	#ifdef GUM_DEBUG
		atexit(freeze);
	#endif

	gum_lexer_init("src/Main.gum");
	gum_token_t token;
	while ((token = gum_lexer_next()).type != GUM_TOKEN_EOF) {
		printf("%i\n", token.type);
	}
}