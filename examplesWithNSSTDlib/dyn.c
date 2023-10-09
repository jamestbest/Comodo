asm ("ldr r13, =$0x10000");

#include "../nsstdlib.h"


__attribute__((optimize("O0"))) int main(){
    putintln(0x010000);

    heapCreate();

    int a = 12;

    char* str = malloc(16);

    putint((int)str);

    str[7] = 'a';

    char* buff = malloc(3);
    getstring('\n', 3, buff);
    print("This is the pointer to the buffer");
    putintln((int)buff);
    int width = str2int(buff);
    println(buff);
    getstring('\n', 3, buff);
    println(buff);
    int height = str2int(buff);

    putintln(width);
    putintln(height);

    println("Creating a grid of the specified size");

    char (*grid)[width] = malloc(width * height);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            grid[i][j] = i + j;
        }
    }

    grid[5][6] = 'a';

    int* myints = malloc(8);
    println("------------------");
    putintln((int)myints);
    free(myints);

    int* myotherints = malloc(8);
    println("------------------");
    putintln((int)myotherints);
    free(myotherints);

    heapPrint();

    free(buff);
    free(str);
    free(grid);

    heapPrint();

    reset(0);

    return 0;
}