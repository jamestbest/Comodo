#pragma GCC push_options
#pragma GCC optimize ("O0")

#include "nsstdlib.h"

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
    asm ("swi 2":::);
    asm ("mov r15, #0":::);
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

#define heapstart   0x010000
#define heapend     0x100000

typedef struct Crate {
    char used;
    struct Crate* next;
    int size;
} Crate;

void heapCreate() {
    /* 
     * usedflag ptr2next  sizeofdata  data
     * byte     4 bytes   4 bytes     ...
     */

    void* head = (void *) heapstart;

    *(Crate *)head = (Crate){0,0, heapend - heapstart - sizeof(Crate)};
}


#define rmlessthan8 0b1111111111111000
void* malloc_debug(unsigned int bytes) {
    /* Find next available Crate of memory
     * Split off the amount needed
     * Move the Crates header
     */
    println("~~~~~~~~~~NEW MALLOC CALL~~~~~~~~~~~~~~");

    Crate* cptr = (Crate *) heapstart;

    bytes = (bytes & rmlessthan8) < bytes ? bytes + 8 & rmlessthan8 : bytes & rmlessthan8;

    print("This is the bytes requested and aligned");
    putintln(bytes);

    while (cptr->used || cptr->size < bytes) {
        if (cptr->next == 0) return 0;
        cptr = cptr->next;
    }

    print("This is the value of the found buffer pointer: ");
    putintln((int)cptr);

    //found the next Crate
    if (cptr->size < (sizeof(Crate) << 1) + bytes) {
        println("Using the buffer instead of splitting");
        //the buffer should not be split up as it cannot hold another Crate header
        cptr->used = 1;
        return cptr + sizeof(Crate);
    }

    println("Splitting the buffer");
    println("The start of the old buffer is changing to have the following value of size");
    putintln(bytes);

    //split the buffer into two new ones
    int add = sizeof(Crate) + bytes;
    print("This is how many bytes we are adding: ");
    putintln(add);
    Crate* newcptr = (Crate *) ((char*)cptr + add);

    print("This is the value of the newcptr: ");
    putintln((int)newcptr);
    asm volatile("crateVals:");
    *newcptr = (Crate){0, 0, cptr->size - sizeof(Crate) - bytes};
    cptr->used = 1;
    cptr->size = bytes;
    cptr->next = newcptr;

    println("~~~~~~~~~~ENDOF MALLOC CALL~~~~~~~~~~~~~~");

    return (Crate*) ((char*)cptr + sizeof(Crate));
}

void* malloc(unsigned int bytes) {
    /* Find next available Crate of memory
     * Split off the amount needed
     * Move the Crates header
     */
    Crate* cptr = (Crate *) heapstart;

    bytes = (bytes & rmlessthan8) < bytes ? bytes + 8 & rmlessthan8 : bytes & rmlessthan8;

    while (cptr->used || cptr->size < bytes) {
        if (cptr->next == 0) return 0;
        cptr = cptr->next;
    }

    //found the next Crate
    if (cptr->size < (sizeof(Crate) << 1) + bytes) {
        println("Using the buffer instead of splitting");
        //the buffer should not be split up as it cannot hold another Crate header
        cptr->used = 1;
        return cptr + sizeof(Crate);
    }

    //split the buffer into two new ones
    int add = sizeof(Crate) + bytes;
    Crate* newcptr = (Crate *) ((char*)cptr + add);

    asm volatile("crateVals:");
    *newcptr = (Crate){0, 0, cptr->size - sizeof(Crate) - bytes};
    cptr->used = 1;
    cptr->size = bytes;
    cptr->next = newcptr;

    return (Crate*) ((char*)cptr + sizeof(Crate));
}

int free(void* ptr) {
    Crate* cptr = ptr - sizeof(Crate);
    cptr->used = 0;
    //could attempt some resizing stuff here as well. If I want to merge blocks behind I will need a ptr to the previous
    return 0;
}

#pragma GCC pop_options