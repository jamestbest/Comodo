#include "../Includes/nsstdlib.h"

int main() {
	char* out = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
	for (int i = 0; i < 20; i++) {
		println(out);
	}

	out = "----------------------------------\n";
    putintln(len(out));
	int length = len(out) * 20 + 1;
	char* outfull = malloc(length);

	for (int i = 0; i < 20; i++) {
		memcpy(out, outfull + i * len(out), len(out));
	}

	outfull[length - 1] = '\0';
	println(outfull);
}
