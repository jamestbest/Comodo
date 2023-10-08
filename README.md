# Comodo
This is a repo with information/files for compiling C programs to an ELF format that the Komodo emulator can load and run

# Comodo's gone dynamic! 🥳

## C file
There are some fairly major caveats to this compilation, the most important being that you CANNOT use anything from the standard library as it will not be loaded.  
At the start of your program you should include `asm ("ldr r13, =$0x10000");` This will set the stack pointer to 10000 as there is only 1MB of memory and accessing addresses outside the bound will spit out 12345678 if you try to read to it.

## Cross-compiler
In order to compile the C program you will need a cross-compiler for ARM  
`sudo apt-get install gcc-arm-linux-gnueabi`  

## Compiling
When compiling use this command  
`arm-linux-gnueabi-gcc <Cfilename> -nostartfiles -nostdlib -nolibc -nodefaultlibs -o <outputFileName> -mcpu=arm7tdmi -T link.lds`  

`-nostartfiles -nostdlib -nolibc -nodefaultlibs` This will disable all of the C libraries that are usually included, I don't think they're all needed I think some probably cover the rest but I'm not testing it.

`-mcpu=arm7tdmi` This is one of the supported architectures listed [here](https://studentnet.cs.manchester.ac.uk/resources/software/komodo/) under target systems

`-T link.lds` This will override the normal linker script. More info below

When compiling with the [nsstdlib](nsstdlib.md) use this command
`arm-linux-gnueabi-gcc <Cfilename> <nsstdlib> -nostartfiles -nostdlib -nolibc -nodefaultlibs -o <outputFileName> -mcpu=arm7tdmi -T link.lds -Wno-builtin-declaration-mismatch`

`<nsstdlib>` This is the file that contains the nsstdlib code.

`-Wno-builtin-declaration-mismatch` disables the warning about redefining the std lib functions

## Linker script
The linker script is just the basic script but with the .text section moved to 0x0. There is currently a bug that appears only on one machine I've tried where PHDR segment not covered by LOAD segment is displayed when compiling. I think this has to do with me removing all of the header data by moving the text section. Or at least I think I'm removing the header data, I'm not really sure.  
The linker script also now changes the entry function to `main`, if there is no main defined within your file it will default to 0x0, and the linker may move your functions around, so it is advisable to have a main. There are no arguments passed to the program and so you don't need argc, and argv.

## Loading into Komodo
Once the file has been compiled to an ELF you can just LOAD it in KMD.
If all has gone well the start of the program should be at addr. 0x0 and so you can just reset Komodo and run. You can find the end of the main function by looking for BX R14 in the assembly view.  

## NSSTDlib
Included in the repo is the "not so standard library". 
Information on interacting with these functions can be found in the [nsstdlib.md](nsstdlib.md).  
To use the nsstdlib include the headerfile at the top of the C file and include the file in the compilation process, described [here](#Compiling)

## Using GCC optimisations
From testing different -O levels GCC destroys the inline assembly functions defined in the nsstdlib and so the file has a #pragma optimize("O0") to exclude it from any optimisations.  
When using the `asm("ldr r13, =$0x10000")` at the start of the file with optimisations the functions may get moved around moving the asm instruction, and so place `__attribute__((optimize("O0")))` infront of your main function declaration to exclude it from all optimisation.

## Extra files
Also included are some simple examples with their compiled versions, as well as some examples that use the nsstdlib.

## TODO
I have not yet tried to have multiple C files that are then linked. I think this will work just fine, but it is not yet tested.    
Is there a better way for when using optimisation than excluding the main function as it holds the asm instruction to set the stack pointer? 
