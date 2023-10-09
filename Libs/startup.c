
extern void main();

#include "nsstdlib.h"

asm ("ldr r13, =$0x10000"); //hopefully temp until I get the linker script working //set sp

void _start() {
	heapCreate();
	
	main();
	
	resets();
}
