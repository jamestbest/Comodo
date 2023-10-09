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

char* strcat(char *stra, char *strb) {
    int lena = len(stra);
    int lenb = len(strb);

    char* strc = malloc(lena + lenb + 1);

    if (strc == 0) {
        println("Failed to malloc in strcat");
        return 0;
    }
    
    for (int i = 0; i < lena; i++) {
        strc[i] = stra[i];
    }
    for (int i = 0; i < lenb; i++) {
        strc[i + lena] = strb[i];
    }

    strc[lena + lenb] = '\0';
    return strc;
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
    print("Program exited with err code: ");
    putintln(err);

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

#define rmlessthan8 0xFFF8
void memcpy(void* src, void* dest, unsigned int bytes) {
    unsigned int mfour = bytes & rmlessthan8;
    unsigned int extra = bytes & 0x7;

    for (int i = 0; i < mfour / 4; i++) {
        int srcword = *(((int*)src) + i);
        *((int*)dest+i) = srcword;
    }

    for (int i = 0; i < extra; i++) {
        char srcchar = *(((char*)src) + i);
        *((char*)dest + i) = srcchar;
    }
}

#define MINBUFFSIZE 8
static char* getstringdyn(char terminator) {
    //attempt to grow the buffer
    int buffSize = MINBUFFSIZE;
    char* buff = malloc(buffSize);

    int index = 0; char inp = getchar();

    while (inp != terminator) {
        if (index >= buffSize) {
            int newbuffsize = buffSize << 1;
            char* bufftemp = malloc(newbuffsize);
            if (bufftemp == 0) {buff[buffSize - 1] = '\0'; return buff;}
            memcpy(buff, bufftemp, buffSize);
            free(buff);
            buff = bufftemp;
            buffSize = newbuffsize;
        }
        buff[index++] = inp;
        inp = getchar();
    }
    buff[(min(index, buffSize - 1))] = '\0';
    return buff;
}
#undef MINBUFFSIZE

char* getstring(char terminator, int maxSize) {
    if (maxSize == -1) return getstringdyn(terminator);

    char* buff = malloc(maxSize); 

    int index = 0; 
    char inp = getchar();

    while (inp != terminator && index < maxSize) {
        buff[index++] = inp;
        inp = getchar();
    }
    buff[min(index, maxSize - 1)] = '\0';

    return buff;
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

// HEAP FUNCTIONS

static void cratePrint(Crate* cptr) {
    println("----------CRATE---------");
    print("Found new block at mem addr: "); putintln((int)cptr);
    println(cptr->used ? "Block is in use" : "Block is availible");
    print("Size: "); putintln((int) (cptr->size));
    print("PTR to next: "); putintln((int) (cptr->next));
    print("PTR to prev: "); putintln((int) (cptr->prev));
}

static void mergeCrates(Crate* c1, Crate* c2) {
    c1->next = c2->next;
    c2->next->prev = c1; //c1 <-> c2 <-> c3   ==>    c1 <-> c3
    c1->size += sizeof(Crate) + c2->size;   
}

static void mergeCrates_debug(Crate* c1, Crate* c2) {
    //c1 < c2
    println("~~~~~~~~~~Merging crates info below~~~~~~~~~");
    cratePrint(c1);
    cratePrint(c2);
    println("Now merging");
    mergeCrates(c1, c2);
    cratePrint(c1);
    cratePrint(c2);
    println("~~~~~~~~~~Crate merge end~~~~~~~~~~");
}

void heapCreate() {
    /* 
     * usedflag ptr2next  ptr2prev  sizeofdata  data
     * byte     4 bytes   4 bytes   4 bytes     ...
     */

    void* head = (void *) heapstart;

    *(Crate *)head = (Crate){0,0,0, heapend - heapstart - sizeof(Crate)};
}

void* malloc_debug(unsigned int bytes) {
    /* Find next available Crate of memory
     * Split off the amount needed
     * Move the Crates header
     */
    println("~~~~~~~~~~NEW MALLOC CALL~~~~~~~~~~~~~~");

    Crate* cptr = (Crate *) heapstart;

    bytes = (bytes & rmlessthan8) < bytes ? (bytes + 8) & rmlessthan8 : bytes & rmlessthan8;

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
    *newcptr = (Crate){0, 0, cptr, cptr->size - sizeof(Crate) - bytes};
    cptr->used = 1;
    cptr->size = bytes;
    cptr->next = newcptr;

    println("This is the value of the pointer that will be returned: ");
    putintln((int)(((char*)cptr + sizeof(Crate))));
    putintln(sizeof(Crate));

    println("~~~~~~~~~~ENDOF MALLOC CALL~~~~~~~~~~~~~~");

    return (Crate*) ((char*)cptr + sizeof(Crate));
}

void* malloc(unsigned int bytes) {
    /* Find next available Crate of memory
     * Split off the amount needed
     * Move the Crates header
     */
    Crate* cptr = (Crate *) heapstart;

    bytes = (bytes & rmlessthan8) < bytes ? (bytes + 8) & rmlessthan8 : bytes & rmlessthan8;
    
    while (cptr->used || cptr->size < bytes) {
        if (cptr->next == 0) {
            println("!!!!!!!!!!Malloc failed to find mem addr!!!!!!!");
            return 0;
        }
        cptr = cptr->next;
    }

    //found the next Crate
    if (cptr->size < (sizeof(Crate) << 1) + bytes) {
        //the buffer should not be split up as it cannot hold another Crate header
        cptr->used = 1;
        return cptr + sizeof(Crate);
    }

    //split the buffer into two new ones
    int add = sizeof(Crate) + bytes;
    Crate* newcptr = (Crate *) ((char*)cptr + add);

    *newcptr = (Crate){0, 0, cptr, cptr->size - sizeof(Crate) - bytes};
    cptr->used = 1;
    cptr->size = bytes;
    cptr->next = newcptr;

    return (Crate*) ((char*)cptr + sizeof(Crate));
}

void heapPrint() {
    /*
     * This function serves to traverse and print out information on the current state of the heap
     */
    println("##########HEAP PRINT############");

    Crate* cptr = (Crate*) heapstart;

    while (cptr != 0) {
        cratePrint(cptr);
        cptr = cptr->next;
    } 

    println("########HEAP PRINT END########");
}

void heapClean() {
    int* heap = (int*) heapstart;
    for (int i = 0; i<(heapend - heapstart - 4) / 4; i++) *(heap + i) = 0;
}

int free(void* ptr) {
    println("$$$$$$$$$$$$$$$Attempting to free$$$$$$$$$$$$$$$$");

    Crate* cptr = ptr - sizeof(Crate);

    putintln((int)cptr);

    cptr->used = 0;

    Crate* left = cptr;
    Crate* right = cptr;

    if (cptr->next->used == 0) {
        right = cptr->next;
    }
    if (cptr->prev->used == 0) {
        left = cptr->prev;
    }

    if (left != right) mergeCrates(left, right);

    println("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$");
    return 0;
}

#pragma GCC pop_options