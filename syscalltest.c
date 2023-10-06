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
    asm ("mov r14, #0":::);
    asm ("swi 2":::);
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

char getchar() {
    char out = 'f';
    asm (
        "swi 1;"
        "mov %0, %%r0;"
        : "=r" (out)
        :
        : "r0"
    );
    return out;
}

int min(int a, int b) {
    return a < b ? a : b;
}

int max(int a, int b) {
    return a > b ? a : b;
}

#define MIN_SIZE 20
void getstring(char terminator, int maxSize, char* buff) {
    char inp = getchar();

    int index = 0;
    while (inp != terminator && index < maxSize) {
        buff[index++] = inp;
        inp = getchar();
    }
    buff[min(index, maxSize - 1)] = '\0';
}