#include <gum/string.h>

int main(int argc, char* argv[]) {
	gum_string_t string;
	gum_string_create(&string);
	gum_string_add(&string, 0, -1, "Hello,");
	gum_string_add(&string, -1, -1, " World!");
	gum_string_add(&string, -1, 6, "\0test\n");
	gum_string_remove(&string, 4, 2);
	fwrite(string.data, string.size, 1, stdout);
}