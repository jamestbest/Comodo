asm ("ldr r13, =$0x10000");

int main(int argc,  char **argv) {
	int a = 1;
	int b = 2;

	int c = a + b;
	return c;
}