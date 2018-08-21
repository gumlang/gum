#include <gum/string.h>
#include <locale.h>
#include <windows.h>
#include <stdlib.h>

int main() {
	gum_string_t* string = gum_string_create();
	gum_string_add(string, 0, -1, "Hello,");
	gum_string_add(string, -1, -1, " World!");
	gum_string_add(string, -1, 5, "\0test");
	gum_string_remove(string, 4, 2);
	fwrite(gum_string_data(string), gum_string_size(string), 1, stdout);
	getchar();
	return 0;
}