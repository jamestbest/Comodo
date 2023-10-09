asm ("ldr r13, =$0x10000");

#include "../nsstdlib.h"


__attribute__((optimize("O0"))) int main() {
    int i = 0x10000;
    for (;;i++) {
        int a = *(int*)i;

        if (a == 0x12345678) break;
    }

    println("Ended at: ");
    putintln(i);

    reset(1);
}