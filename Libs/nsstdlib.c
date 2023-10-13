#pragma GCC push_options
#pragma GCC optimize ("O0")
#pragma GCC diagnostic ignored "-Wunused-function"

#include "nsstdlib.h"

void heapCreate(unsigned int heap_start, unsigned int heap_end);
void heapCreate_debug(unsigned int heap_start, unsigned int heap_end);
void resets();
void reset(int errcode);

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

    resets();
}

void resets() {
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

#define maxhexbuff 9
void puthex(unsigned int a) {
    /* 
     * should be unsigned
     */

    char buff[9];

    if (a <= 0) {putint(a); return;}

    int count = 0;
    int tempa = a;
    while (tempa != 0) {count++; tempa >>= 4;}

    for (int i = 0; i < count; i++) {
        int rem = a & 15;
        a = a >> 4;
        char out = (char) (rem > 9 ? (rem + 55) : (rem + 48));
        buff[count - i - 1] = out;
    }

    buff[count] = '\0';

    print(((char*)buff));

    return;
}

void puthexln(unsigned int a) {
    puthex(a);
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

void memcpy_bytes(void* src, void* dst, unsigned int bytes) {
    for (int i = 0; i < bytes; i++) {
        *((char*)dst + i) = *((char*)src + i);
    }
}

#define rmlessthan8 0xFFF8
#define rmlessthan4 0xFFFC
void memcpy(void* src, void* dst, unsigned int bytes) { //writes must be aligned
    unsigned int align = 0;

    int src_misalign = (long int) src & 0x3;
    int dst_misalign = (long int) dst & 0x3;

    if (src_misalign != dst_misalign) {
        //if the src and destination addresses are misaligned by different amounts then we cannot write any words to them
        memcpy_bytes(src, dst, bytes);
        return;
    }

    for (int i = 0; i < src_misalign; i++) {
        //align the addresses by just writing bytes
        *((char*)dst + i) = *((char*)src + i);
    }

    bytes -= align;
    unsigned int mfour = bytes & rmlessthan4; //find the number of words that we can write to the dst
    unsigned int extra = bytes & 0x3; //the number of bytes to write after
    src += align;
    dst += align;

    for (int i = 0; i < (mfour >> 2); i++) {
        *((int*)dst + i) = *((int*)src + i);
    }

    for (int i = mfour; i < extra + mfour; i++) {
        *((char*)dst + i) = *((char*)src + i);
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

    char* buff = malloc(maxSize + 1); 

    int index = 0; 
    char inp;

    do {
        inp = getchar();
        buff[index++] = inp;
    } while (inp != terminator && index < maxSize);
    buff[inp == terminator ? index - 1 : index] = '\0';

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
    print("Found new block at mem addr: "); puthexln((int)cptr);
    println(cptr->used ? "Block is in use" : "Block is availible");
    print("Size: "); putintln((int) (cptr->size));
    print("PTR to next: "); puthexln((int) (cptr->next));
    print("PTR to prev: "); puthexln((int) (cptr->prev));
}

static void mergeCrates(Crate* c1, Crate* c2) {
    c1->next = c2->next;

    if (c2->next != 0) {
        c2->next->prev = c1;//c1 <-> c2 <-> c3   ==>    c1 <-> c3
    }
    c1->size += sizeof(Crate) + c2->size;
}

static void mergeCrates_debug(Crate* c1, Crate* c2) {
    //c1 < c2
    println("~~~~~~~~~~Merging crates info below~~~~~~~~~");
    heapPrint();
    cratePrint(c1);
    cratePrint(c2);
    println("Now merging");
    mergeCrates(c1, c2);
    cratePrint(c1);
    cratePrint(c2);
    heapPrint();
    println("~~~~~~~~~~Crate merge end~~~~~~~~~~");
}

unsigned int heapstart;
unsigned int heapend;
void heapCreate_debug(unsigned int heap_start, unsigned int heap_end) {
    /*
 * usedflag ptr2next  ptr2prev  sizeofdata  data
 * byte     4 bytes   4 bytes   4 bytes     ...
 */
    heapstart = heap_start;
    heapend = heap_end;

    println("Creating heap, start and end addr: ");
    puthexln(heap_start);
    puthexln(heap_end);
    println("Total size in Kb");
    putintln((heap_end - heap_start) / 1024);

    *(Crate *)heap_start = (Crate){0,0,0, heap_end - heap_start - sizeof(Crate)};
}


void heapCreate(unsigned int heap_start, unsigned int heap_end) {
    /*
     * usedflag ptr2next  ptr2prev  sizeofdata  data
     * byte     4 bytes   4 bytes   4 bytes     ...
     */
    heapstart = heap_start;
    heapend = heap_end;
    *(Crate *)heap_start = (Crate){0,0,0, heap_end - heap_start - sizeof(Crate)};
}

void* malloc(unsigned int bytes) {
        /* Find next available Crate of memory
     * Split off the amount needed
     * Move the Crates header
     */
    Crate* largeCratePtr = (Crate *) heapstart;

    bytes = (bytes & rmlessthan8) < bytes ? (bytes + 8) & rmlessthan8 : bytes & rmlessthan8;
    
    while (largeCratePtr->used || largeCratePtr->size < bytes) {
        if (largeCratePtr->next == 0) {
            return 0;
        }
        largeCratePtr = largeCratePtr->next;
    }

    //found the next Crate
    if (largeCratePtr->size < (sizeof(Crate) << 1) + bytes) {
        //the buffer should not be split up as it cannot hold another Crate header
        largeCratePtr->used = 1;

        return (Crate*) ((char*)largeCratePtr + sizeof(Crate));
    }
    
    //split the buffer into two new ones
    int add = sizeof(Crate) + bytes;
    Crate* newcptr = (Crate *) ((char*)largeCratePtr + add);

    if (largeCratePtr->next) {
        largeCratePtr->next->prev = newcptr;
    }

    *newcptr = (Crate){0, largeCratePtr->next, largeCratePtr, largeCratePtr->size - sizeof(Crate) - bytes};

    largeCratePtr->used = 1;
    largeCratePtr->size = bytes;
    largeCratePtr->next = newcptr;

    return (Crate*) ((char*)largeCratePtr + sizeof(Crate));
}


void* malloc_debug(unsigned int bytes) {
    /* Find next available Crate of memory
     * Split off the amount needed
     * Move the Crates header
     */
    println("~~~~~~~~~~NEW MALLOC CALL~~~~~~~~~~~~~~");
    heapPrint();

    Crate* largeCratePtr = (Crate *) heapstart;

    bytes = (bytes & rmlessthan8) < bytes ? (bytes + 8) & rmlessthan8 : bytes & rmlessthan8;
    
    while (largeCratePtr->used || largeCratePtr->size < bytes) {
        if (largeCratePtr->next == 0) {
            println("!!!!!!!!!!Malloc failed to find mem addr!!!!!!!");
            heapPrint();
            return 0;
        }
        largeCratePtr = largeCratePtr->next;
    }

    print("This is the value of the found buffer pointer: ");
    puthexln((int)largeCratePtr);

    //found the next Crate
    if (largeCratePtr->size < (sizeof(Crate) << 1) + bytes) {
        println("Using the buffer instead of splitting");
        //the buffer should not be split up as it cannot hold another Crate header
        largeCratePtr->used = 1;

        print("This is the value of the pointer that will be returned: ");
        puthexln((int)(((char*)largeCratePtr + sizeof(Crate))));

        heapPrint();
        println("~~~~~~~~~~ENDOF MALLOC CALL~~~~~~~~~~~~~~");

        return (Crate*) ((char*)largeCratePtr + sizeof(Crate));
    }

    println("Splitting the buffer");

    //split the buffer into two new ones
    int add = sizeof(Crate) + bytes;
    print("This is how many bytes we are adding: ");
    putintln(add);
    Crate* newcptr = (Crate *) ((char*)largeCratePtr + add);

    println("Info on NEW Crate, the one with the rest of the memory");
    print("addr: "); puthexln((int)newcptr);
    print("next: "); puthexln((int)largeCratePtr->next);
    print("prev: "); puthexln((int)largeCratePtr);
    print("size: "); putintln(largeCratePtr->size - sizeof(Crate) - bytes);

    largeCratePtr->next->prev = newcptr;

    *newcptr = (Crate){0, largeCratePtr->next, largeCratePtr, largeCratePtr->size - sizeof(Crate) - bytes};

    println("The old buffer is changing its information to the following");
    print("addr: "); puthexln((int)largeCratePtr);
    print("next: "); puthexln((int) newcptr);
    print("prev: "); puthexln((int) largeCratePtr->prev);
    print("size: "); putintln(bytes);
    largeCratePtr->used = 1;
    largeCratePtr->size = bytes;
    largeCratePtr->next = newcptr;

    print("This is the value of the pointer that will be returned: ");
    puthexln((int)(((char*)largeCratePtr + sizeof(Crate))));
    putintln(sizeof(Crate));

    heapPrint();
    println("~~~~~~~~~~ENDOF MALLOC CALL~~~~~~~~~~~~~~");

    return (Crate*) ((char*)largeCratePtr + sizeof(Crate));
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

    heapCreate(heapstart, heapend);
}

int free(void* ptr) {
    Crate* cptr = ptr - sizeof(Crate);

    cptr->used = 0;

    Crate* left = cptr;
    Crate* right = cptr;

    if (cptr->next->used == (char)0) {
        right = cptr->next;
    }
    if (cptr->prev->used == (char)0) {
        left = cptr->prev;
    }

    if (left != right) mergeCrates(left, right);

    return 0;
}

#pragma GCC pop_options
