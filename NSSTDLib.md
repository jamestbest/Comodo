# NSSTDLIB Information
## Rules of interaction
The NSSTDlib will create a heap that is used in dynamic memory allocation. Compile with runWS.sh in order to correctly set up the NSSTDlib details found [here](README.md#Compiling)

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

| Subroutine                  | Description                                        | SWIs |
|-----------------------------|----------------------------------------------------|------|
| void putint(int a)          | Output a to the features tab                       | 4    |
| void putchar(char a)        | Output a to the features tab                       | 0    |
| void print(char* str)       | Print a null terminated string to the features tab | 3    |
| void puthex(unsigned int a) | Outputs the hex version of a                       | 3    |

### String functions

| Subroutine                                  | Description                                                                          | SWIs |
|---------------------------------------------|--------------------------------------------------------------------------------------|------|
| char* getstring(char terminator, int bytes) | Reads input until terminator or max bytes reached                                    | 1    |
| char* getstring(char terminator, -1)        | Reads input until terminator reached, dynamically changes buffer size to accommodate | 1    |
| int streq(char* stra, char* strb)           | Returns 1 if both the strings are equal else returns 0                               | -    |
| char* strcat(char* stra, char* strb)        | Concatenates strings a and b and returns the result as a new string                  | -    |
| int str2int(char* str)                      | Returns the integer version of the inputted null terminated string                   | -    |
| int len(char* str)                          | Returns the length of the inputted null terminated string                            | -    |

## Maths functions

| Subroutine                  | Description                    | SWIs |
|-----------------------------|--------------------------------|------|
| int min(int a, int b)       | Returns the minimum of a and b | -    |
| int max(int a, int b)       | Returns the maximum of a and b | -    |
| int pow(int base, int expo) | Returns base ** expo           | -    |
| int ror(int a, int rep)     | Rotates a right rep times      | -    |

## Dynamic memory allocation
In order to support dynamic memory allocation the NSSTDlib will create a heap from the end of your program to 0x100000, this provides ~1MB of heap space.

Calls to malloc will have their requested bytes set to the closest multiple of 8. Each piece of memory added to the heap uses a Crate struct to hold information on the section, i.e. if its being used, its size, and a pointer to the next crate, as well as one to the previous. This means there is 4 bytes of header before each malloc point and so the effective amount of dynamic memory will depend on how small your calls to malloc are.

| Subroutine                              | Description                                                                                                                   | SWIs |
|-----------------------------------------|-------------------------------------------------------------------------------------------------------------------------------|------|
| void* malloc(unsigned int xbytes)       | Allocated xbytes bytes on the heap and returns a pointer to the start                                                         | -    |
| void* malloc_debug(unsigned int xbytes) | Same as malloc put with printed debug information about allocation                                                            | 3, 4 |
| void free(void* ptr)                    | Frees the memory from the heap, potentially merging adjacent Crates                                                           | -    |
| void heapClean()                        | Zeros all memory addresses in the heap. Used for debugging by removing excess information. Should be called before heapCreate | -    |
| void heapPrint()                        | Traverses the heap and prints information about the Crates                                                                    | 3, 4 |


## Do not call
Below is a list of functions that the NSSTDlib uses internally, these should not be used

| Subroutine                                                      | Description                                                    | SWIs |
|-----------------------------------------------------------------|----------------------------------------------------------------|------|
| void heapCreate(unsigned int heap_start, unsigned int heap_end) | Sets up the heap header Crate with its size                    |      |
| void reset(int err)                                             | Resets the PC and halts KMD. Also prints out the error code    | 2, 3 |
| void resets()                                                   | Resets the PC and halts KMD silently, does not output err code | 2    |

