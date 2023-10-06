asm ("ldr r13, =$0x10000");

#include "syscalltest.h"

int main() {
    int out = 1; int inp = 2;
    putchar('c');
    putchar('\n');

    char *str = "Hello world!\n\0";

    print(str);

    for (int i = 0; i < 10; i++) {
        char *out = "The value of i is \0";
        print(out);
        putintln(i);
    }

    char* a = "Hello";
    char* b = " World!";
    int newlen = len(a) + len(b) + 1;
    char c[newlen];

    putintln(newlen);

    strcat(a,b,c);

    println(c);

    char chr = getchar();

    putcharln(chr);

    print("Please enter your name: ");

    int size = 10;
    char buff[size];
    getstring('\n', size, buff);

    print("Your name is: ");
    println(buff);

    println(streq(buff, "james") ? "You are admin" : "You are not admin");

    reset(0);
}

int a() {
    return 1;
}