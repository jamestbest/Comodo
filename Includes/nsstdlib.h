#pragma once

char getchar();

void putchar (char a);
void putcharln (char a);
void putint (int a);
void putintln (int a);
void print (char *str);
void println (char *str);
void puthex(unsigned int a);
void puthexln(unsigned int a);

void resets(); //reset silently
void reset(int err); //outputs exit code

char* getstring(char terminator, int maxSize);
int streq(char *stra, char *strb);
char* strcat(char *stra, char *strb);
int str2int(char *str);
int len(char *str);

int min(int a, int b);
int max(int a, int b);
int pow(int base, int expo);
int ror(int a, int rep);

typedef struct Crate {
    char used;
    struct Crate* next;
    struct Crate* prev;
    int size;
} Crate;

void heapCreate();
void heapClean();
void* malloc(unsigned int bytes);
void* malloc_debug(unsigned int bytes);
int free(void* ptr);
void heapPrint();

#define heapstart   0x010000
#define heapend     0x100000