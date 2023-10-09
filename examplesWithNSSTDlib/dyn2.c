asm ("ldr r13, =$0x10000");

#include "../nsstdlib.h"


__attribute__((optimize("O0"))) int main(){
    heapCreate();

    heapPrint();

    char* str = getstring('\n', 3);
    char* str2 = getstring('\n', -1);

    print("PRINTING");
    println(str);
    println(str2);

    heapPrint();

    reset(0);
    return 0;
}