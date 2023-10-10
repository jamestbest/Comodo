#include "nsstdlib.h"

int main(){
    int a = 12;

    char* str = malloc(16);

    putint((int)str);

    str[7] = 'a';

    
    char* buff = getstring('\n', 3);
    print("This is the pointer to the buffer");
    putintln((int)buff);
    int width = str2int(buff);
    println(buff);
    free(buff);
    buff = getstring('\n', 3);
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

    return 0;
}
