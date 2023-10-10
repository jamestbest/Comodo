#include "nsstdlib.h"

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
    putintln(newlen);

    char* c = strcat(a,b);

    println(c);

    println("enter a character");

    char chr = getchar();

    putcharln(chr);

    print("Please enter your name: ");

    int size = 10;
    char* buff = getstring('\n', size);

	putintln(len(buff));
	putintln(len("james"));

    print("Your name is: ");
    println(buff);

    putintln(streq("james", "james"));

    println(streq(buff, "james") ? "You are admin" : "You are not admin");

    return 0;
}

int a() {
    return 1;
}
