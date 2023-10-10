
extern void main();

asm("ldr r13, =$0x010000");


void _start() {
	main();

	asm("swi 2");
	asm("mov r15, #0");
}
