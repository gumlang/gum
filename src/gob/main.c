#include "input.h"
#include <stdlib.h>

#ifdef GUM_DEBUG
	static void freeze() {
		puts("Press enter to continue");
		getchar();
	}
#endif

int main() {
	#ifdef GUM_DEBUG
		atexit(freeze);
	#endif

	gum_input_init("src/Main.gum");

	gum_char_t c;
	do {
		c = gum_input_next();
		if (c.c == '!') {
			gum_input_error(c, "This is an error");
		}
	} while (c.c != -1);
}