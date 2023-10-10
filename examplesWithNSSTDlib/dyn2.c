#include "nsstdlib.h"

int main(){
    heapPrint();

    char* str = getstring('\n', 3);
    char* str2 = getstring('\n', -1);

    print("PRINTING");
    println(str);
    println(str2);

    heapPrint();

    return 0;
}
