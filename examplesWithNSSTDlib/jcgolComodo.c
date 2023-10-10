#include "nsstdlib.h"

int width = 18;
int height = 18;

#define MAXWIDTH 30
#define MAXHEIGHT 30

void getdims();
void update();
void gengrid(char grid[height][width]);
void printgrid(char grid[height][width]);
void genrandom(char grid[height][width]);
void genfromdrawing(char grid[height][width]);
void updategrid(char active[height][width], char passive[height][width]);
void loop(char active[height][width], char passive[height][width]);
int countNeighbours(int row, int col, char active[height][width]);
int isvalidindex(int row, int col);

int main(){
    char activeGrid[MAXHEIGHT][MAXWIDTH];
    char passiveGrid[MAXHEIGHT][MAXWIDTH];

    getdims();

    gengrid(activeGrid);

    printgrid(activeGrid);

    loop(activeGrid, passiveGrid);

    return 0;
}

void getdims() {
getwidstart:
    println("Please enter a width for the grid");
    char* widthbuff = getstring('\n', 2);
    int wid = str2int(widthbuff);

    if (wid < 0 || wid > MAXWIDTH) {
        println("That is not a valid width");
        goto getwidstart;
    }

getheightstart:
    println("Please enter a height for the grid");
    char* heightBuff = getstring('\n', 2);
    int hei = str2int(heightBuff);

    if (hei < 0 || hei > MAXHEIGHT) {
        println("That is not a vaid height");
        goto getheightstart;
    }

    width = wid;
    height = hei;
}

void initgrid(char grid[height][width]) {
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            grid[row][col] = 0;
        }
    }
}

int getseed() {
    println("Please enter 4 characters");

    int out = 0;
    for (int i = 0; i < 4; i++) {
        char chr = getchar();
        out += (int) chr;
        out <<=  2;
        putchar(chr);
    }
    putchar('\n');
    return out;
}

void gengrid(char grid[height][width]) {
gengridchoice:
    println("Please choose between (r)andom generation of the grid and (d)rawing the grid by hand");
    char choice = getchar();
    if (choice != 'r' && choice != 'd') {
        println("Invalid choice, lets try this again");
        goto gengridchoice;
    }

    switch (choice) {
        case 'r':
            genrandom(grid);
            break;
        case 'd':
            genfromdrawing(grid);
            break;
    }
}

void genfromdrawing(char grid[height][width]) {
    initgrid(grid);
    println("For each 'pixel' in the grid type '1' or '0' to set it to alive/dead respectivly");
    printgrid(grid);
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
gfdloop:
            char inp = getchar();
            if (inp != '1' && inp != '0') goto gfdloop;

            grid[row][col] = inp == '1';

            printgrid(grid);
        }
    }
}

void genrandom(char grid[height][width]) {
    int seed = getseed();
    putintln(seed);

    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            grid[row][col] = seed % 2 == 0;
            seed = ror(seed, 1);
            seed ^= col << min(4, row);
        }
    }
}

void printgrid(char grid[height][width]) {
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            if (grid[row][col] == 1) {
                putchar('X');
            } else {
                putchar('.');
            }
            putchar(' ');
        }
        putchar('\n');
    }

    for (int i = 0; i < width << 1; i++) {
        putchar('-');
    }

    putchar('\n');
}

void loop(char active[height][width], char passive[height][width]) {
    int gridSelect = 1;
    while (1) {
        putintln(gridSelect);
        putintln(!gridSelect);
        updategrid(gridSelect ? active : passive, !gridSelect ? active : passive);

        printgrid(gridSelect ? active : passive);

        gridSelect = !gridSelect;
    }
}

void updategrid(char active[height][width], char passive[height][width]) {
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            int nCount = countNeighbours(row, col, active);
            int isAlive = active[row][col];

            if (isAlive) {
                passive[row][col] = nCount == 2 || nCount == 3;
            } else {
                passive[row][col] = nCount == 3;
            }
        }
    }
}

int offsets[16] = {-1,-1,-1,1,-1,0,0,-1,0,1,1,-1,1,0,1,1};

int countNeighbours(int row, int col, char active[height][width]){
    int count = 0;
    for (int i = 0; i < 16; i+=2) {
        int rowoffset = offsets[i];
        int coloffset = offsets[i + 1];

        if (isvalidindex(row + rowoffset, col + coloffset)) {
            count += active[row + rowoffset][col + coloffset];
        }
    }
    return count;
}

int isvalidindex(int row, int col){
    return row >= 0 && row < height && col >= 0 && col < width;
}
