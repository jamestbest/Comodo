#include "syscalltest.h"

int len(char *str){
    int i = 0;
    while (str[i++] != '\0');
    return i - 1;
}

void strcat(char *stra, char *strb, char *strc) {
    int lena = len(stra);
    int lenb = len(strb);
    
    for (int i = 0; i < lena; i++) {
        strc[i] = stra[i];
    }
    for (int i = 0; i < lenb; i++) {
        strc[i + lena] = strb[i];
    }

    strc[lena + lenb] = '\0';
}

void print(char *str) {
    //print a null terminated string
    asm (
        "mov %%r0, %0;"
        "swi 3;"
        :
        : "r" (str)
        : "r0"
    );
}

void println(char *str) {
    print(str);
    putchar('\n');
}

void reset(int err){
    asm ("swi 2":::);
    asm ("mov r14, #0":::);
}

void putchar (char a) {
    asm (
        "mov %%r0, %0;"
        "swi 0;"
        :
        : "r" (a)
        : "r0"
    );
}

void putcharln (char a) {
    putchar(a);
    putchar('\n');
}

void putint (int a) {
    asm (
        "mov %%r0, %0;"
        "swi 4;"
        :
        : "r" (a)
        : "r0"
    );
}

void putintln (int a) {
    putint(a);
    putchar('\n');
}