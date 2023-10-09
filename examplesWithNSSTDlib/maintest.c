asm ("ldr r13, =$0x10000");

#include "../nsstdlib.h"


__attribute__((optimize("O0"))) int main() {
    heapCreate();

    println("Hello and welcome to the main file for testing the nsstdlib!");

    println("If you plan on using the heap at all you will need to use heapCreate() at the start of your program. Some nsstdlib functions will use the heap.");

    println("First step is to print something to the screen, and it seems we've at least got that working, so next will be just a single character");
    putcharln('h');

    println("Next is user inputs, we're going to first collect a single character from the user, then a string of maxsize 4 (type 4 characters or ENTER key to end), and then a string that can be any size until we reach '\\n");
    char inp = getchar();

    putcharln(inp);

    char* inp2 = getstring('\n', 4);

    println(inp2);

    println("For this large string input, try to type a lot of characters as it will help with the examples later on");

    char* inp3 = getstring('\n', -1);

    println(inp3);

    println("Now some string functions first is streq for the following strings");

    char* stra = "Hello world!";
    char* strb = "Hello w0rld!";

    println(stra);
    println(strb);

    int eq = streq(stra, strb);

    println(eq ? "The string are equal" : "The strings are not equal");

    println("We can also compare to the string that was created earlier");
    eq = streq(stra, inp3);

    println(eq ? "The string are equal" : "The strings are not equal");

    println("We can concatenate string together");

    char* concat = strcat(stra, strb);

    println(concat);

    println("As well as convert a string into an integer");

    char* strnum = "653";

    int intnum = str2int(strnum);

    println(strnum);
    putintln(intnum);

    println("You can also get the length of a string");
    println(inp3);
    putintln(len(inp3));

    println("There are several basic math functions: ");
    int a = 12; int b = 7;
    putint(a); print(" ror "); putint(b); putchar(' '); putintln(ror(a,b));
    putint(a); print(" pow "); putint(b); putchar(' '); putintln(pow(a,b));
    print("min between "); putint(a); putchar(' '); putint(b); print(" is "); putintln(min(a,b));
    print("max between "); putint(a); putchar(' '); putint(b); print(" is "); putintln(max(a,b));

    println("When ending your program use reset() this will allow you to then press run again without reset. If your program does not reach reset() or you have loaded a new version into kmd then use the reset button manually");

    println("There is also dynamic memory allocation, it is used in some of the nsstdlib functions such as strcat but you can call it yourself with malloc()");
    println("Input two numbers with a delimiter of ENTER key. They should be 1-2 digit numbers");

    int wid = str2int(getstring('\n', 3));
    putintln(wid);

    int hei = str2int(getstring('\n', 3));
    putintln(hei);

    char (*grid)[wid] = malloc(wid * hei);

    println("Now we have created an array with total size of: "); putintln(wid * hei);

    println("We can have a look at the heap to see what's been allocated");
    heapPrint();

    println("We should see a lot of different things allocated, this is because every call to getstring() will have its result allocated onto the heap to prevent you from having to create a buffer");
    println("Lets free all of those strings");

    println("Press any key to continue");
    char temp = getchar();

    free(inp2);
    free(inp3);

    //free(wid);
    //free(hei);

    println("We've got a problem at the moment as the code for getting wid and hei doesn't allow use to free the results of getstring()");
    println("We'll move on from this and now free the result of the strcat as well");

    free(concat);

    println("Now lets see what the heap looks like");
    heapPrint();

    println("As you can see some of the Crates have combined into a larger one that is marked as availible. There are still some allocations we haven't freed (two we can't, damned memory leaks)");
    println("We'll now free the grid and see the final state");
    free(grid);

    println("Press any key to continue");
    temp = getchar();

    heapPrint();

    println("Press any key to continue");
    temp = getchar();

    println("We should have a free block at the beginning (at least 8 bytes) that we can try and use ");
    char* freeme = malloc(8);
    println("I've requested 8 bytes of memory from malloc and now we can see where it took it from");
    heapPrint();

    println("Malloc will always try to find the first properly sized block of memory, and as this heap is a linked list it will be at the lowest memory address");

    free(freeme);

    println("That concludes this strange 'tutorial' that was actually incredibly useful at helpling me find and fix bugs.");
    println("Feel free to edit some of the errors/mistakes in this code");
    reset(0);
}