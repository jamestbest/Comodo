asm ("ldr r13, =$0x10000");

#include "syscalltest.h"

int main() {
    int a = 12;
    int out = 1; int inp = 2;
    putchar('c');
    putchar('\n');

    char *str = "Hello world!\n\0";

    print(str);

    for (int i = 0; i < 10; i++) {
        char *out = "The value of i is \0";
        print(out);
        char val = (char)(i+48);
        putchar(val);
        putchar('\n');
    }

    exit(0);
}

int a() {
    return 1;
}