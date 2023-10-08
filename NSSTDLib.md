# NSSTDLIB Information
As of this commit dynamic allocation is not availible and so all nsstdlib functions that create data that is not a primitive that needs to be returned will require a buffer as input.  

## Included functions
### Input functions  
These are functions to collect input from the user  
  - char getchar()                                                      This will return a single character typed into features (swi 1)
  - char getstring(char terminator, int maxSize, char* buffer)          This will continue getting input from the user until the user types the terminator or the maxSize is reached. It is not dynamically allocated and so requires a buffer.

### Output functions
These are functions that will print to the features tab  
All of the following functions have a \<funcName\>ln() variant that will print a newline character after
  - void putint(int a)                                                  This will output the int to features tab (swi 4)
  - void putchar(char a)                                                This will output the single charater to features (swi 0)
  - void print(char* str)                                               Prints a null-terminated string to features (swi 3)

### String functions
  - char getstring(char terminator, int maxSize, char* buffer)          This will continue getting input from the user until the user types the terminator or the maxSize is reached. It is not dynamically allocated and so requires a buffer.
  - int streq(char* stra, char* strb)                                   Returns 1 if both strings are equal and 0 otherwise.
  - void strcat(char* stra, char* strb, char* out)                      Will concatonate stra and strb excluding stra's null terminator. It is not dynamically allocated and so requires a buffer.
  - int str2int(char* str)                                              Returns the integer value of an inputted string
  - int len(char* str)                                                  Returns the length of a null terminated string DOES NOT include the null terminator

## Maths functions
  - int min(int a, int b)                                               Returns the minimum of the two inputted values
  - int max(int a, int b)                                               Returns the maximum of the two inputted values
  - int pow(int base, int expo)                                         Returns base ** expo
  - int ror(int a, int rep)                                             Rotates a rep times

## Misc
  - void reset(int err)                                                 Resets the stack pointer and sets the program counter to 0x0 so that you can just press run again without reset. Error code currently not used.

## Dynamic memory allocation
In order to support dynamic memory allocation the nsstdlib assumes that memory addresses 0x010000 - 0x100000 are a heap. These should not be otherwise interacted with. This gives a program about 960KiB of dynamic memory.

Calls to malloc will have their requested bytes set to the closest multiple of 8. Each piece of memory added to the heap uses a Crate struct to hold information on the section, i.e. if its being used, its size, and a pointer to the next crate. This means there is 3 bytes of header before each malloc point and so the effective amount of dynamic memory will depend on how small your calls to malloc are.

In order to use the heap you must first call heapCreate() this will setup the heap header Crate.  

  - void* malloc(unsigned int bytes)                                    Allocate the number of bytes specified on the heap and return pointer to the memory, if it fails the result will be 0
  - void* malloc_debuf(unsigned int bytes)                              malloc but with debug information printed to the features tab
  - void free(void* ptr)                                                Frees the memory from the heap making it availible to be reused. 