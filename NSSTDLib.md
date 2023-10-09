# NSSTDLIB Information
## Rules of interaction
The nsstdlib makes some assumptions about how memory is being interacted with in your program, for example it assumes that memory addresses 0x

## Included functions
### Input functions
| Subroutine                                  | Description                                                                          | SWIs |
|---------------------------------------------|--------------------------------------------------------------------------------------|------|
| char getchar()                              | Returns the character typed into features tab                                        | 1    |
| char* getstring(char terminator, int bytes) | Reads input until terminator or max bytes reached                                    | 1    |
| char* getstring(char terminator, -1)        | Reads input until terminator reached, dynamically changes buffer size to accommodate | 1    |

### Output functions
These are functions that will print to the features tab  
All the following functions have a \<funcName\>ln() variant that will print a newline character after

| Subroutine            | Description                                        | SWIs |
|-----------------------|----------------------------------------------------|------|
| void putint(int a)    | Output a to the features tab                       | 4    |
| void putchar(char a)  | Output a to the features tab                       | 0    |
| void print(char* str) | Print a null terminated string to the features tab | 3    |

### String functions

| Subroutine                                  | Description                                                                          | SWIs |
|---------------------------------------------|--------------------------------------------------------------------------------------|------|
| char* getstring(char terminator, int bytes) | Reads input until terminator or max bytes reached                                    | 1    |
| char* getstring(char terminator, -1)        | Reads input until terminator reached, dynamically changes buffer size to accommodate | 1    |
| int streq(char* stra, char* strb)           | Returns 1 if both the strings are equal else returns 0                               | -    |
| void strcat(char* stra, char* strb)         | Concatenates strings a and b and returns the result as a new string                  | -    |
| int str2int(char* str)                      | Returns the integer version of the inputted null terminated string                   | -    |
| int len(char* str)                          | Returns the length of the inputted null terminated string                            | -    |

## Maths functions

| Subroutine                  | Description                    | SWIs |
|-----------------------------|--------------------------------|------|
| int min(int a, int b)       | Returns the minimum of a and b | -    |
| int max(int a, int b)       | Returns the maximum of a and b | -    |
| int pow(int base, int expo) | Returns base ** expo           | -    |
| int ror(int a, int rep)     | Rotates a right rep times      | -    |

## Misc

| Subroutine          | Description                  | SWIs |
|---------------------|------------------------------|------|
| void reset(int err) | Resets the PC and halts KMD. | 2    |   

## Dynamic memory allocation
In order to support dynamic memory allocation the nsstdlib assumes that memory addresses 0x010000 - 0x100000 are a heap. These should not be otherwise interacted with. This gives a program about 960KiB of dynamic memory.

Calls to malloc will have their requested bytes set to the closest multiple of 8. Each piece of memory added to the heap uses a Crate struct to hold information on the section, i.e. if its being used, its size, and a pointer to the next crate. This means there is 3 bytes of header before each malloc point and so the effective amount of dynamic memory will depend on how small your calls to malloc are.

In order to use the heap you must first call heapCreate() this will setup the heap header Crate.  

| Subroutine                              | Description                                                                                                                   | SWIs |
|-----------------------------------------|-------------------------------------------------------------------------------------------------------------------------------|------|
| void* malloc(unsigned int xbytes)       | Allocated xbytes bytes on the heap and returns a pointer to the start                                                         | -    |
| void free(void* ptr)                    | Frees the memory from the heap, potentially merging adjacent Crates                                                           | -    |
| void heapCreate()                       | Creates the first Crate header in the heap. Must be called before heap usage                                                  | -    |
| void heapClean()                        | Zeros all memory addresses in the heap. Used for debugging by removing excess information. Should be called before heapCreate | -    |
| void* malloc_debug(unsigned int xbytes) | Same as malloc put with printed debug information about allocation                                                            | 3, 4 |
| void heapPrint()                        | Traverses the heap and prints information about the Crates                                                                    | 3, 4 |
