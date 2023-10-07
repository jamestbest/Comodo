#pragma GCC push_options
#pragma GCC optimize ("O0")

#include "syscalltest.h"

int ror(int a, int rep) {
    int out;
    asm (
        "mov %[out0], %[inp0], ror %[inp1]"
        : [out0] "=r" (out)
        : [inp0] "r" (a), [inp1] "r" (rep)
        :);
    return out;
}

int pow(int base, int expo) {
    int out = 1;

    for (int i = 0; i < expo; i++) {
        out *= base;
    }

    return out;
}

int str2int(char *str){
    int length = len(str);

    int out = 0;
    for (int i = length - 1; i >= 0; i--) {
        int digit = str[i] - 48;
        out += digit * pow(10, length - i - 1);
    }

    return out;
}

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

int streq(char *stra, char *strb) {
    int lena = len(stra);
    int lenb = len(strb);

    if (lena != lenb) {
        return 0;
    }

    for (int i = 0; i < lena; i++) {
        if (stra[i] != strb[i]) {
            return 0;
        }
    }
    return 1;
}

#pragma GCC pop_options