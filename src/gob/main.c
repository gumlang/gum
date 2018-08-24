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
		printf("%04X\n", token.type);
		if (token.type == GUM_TOKEN_NAME || token.type == GUM_TOKEN_STRING) {
			putchar('\t');
			fwrite(token.data.s.data, token.data.s.size, 1, stdout);
			putchar('\n');
			gum_string_destroy(&token.data.s);
		} else if (token.type == GUM_TOKEN_INT) {
			printf("\t%i\n", token.data.i);
		} else if (token.type == GUM_TOKEN_FLOAT) {
			printf("\t%f\n", token.data.f);
		}
	}
}