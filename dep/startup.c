#pragma GCC push_options
#pragma GCC optimize ("O0")
#pragma GCC pop_options

asm ("ldr r13, =$0x10000"); //hopefully temp until I get the linker script working //set sp

extern int main();
extern unsigned __sstack; //__sstack is a pointer to the start of the stack which would normally be 0x100000

#include "nsstdlib.h"

void _start() {
    unsigned int s_val = &__sstack;
    asm (
            "mov %%r13, %[sinit]"
            :
            : [sinit] "r" (s_val)
            : "r13"
    );

	heapCreate();
	
	int errc = main();
	
	reset(errc);
}

#pragma pop_options
