# Comodo
This is a repo with information/files for compiling C programs to an ELF format that the Komodo emulator can load and run

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

When compiling with the [nsstdlib]("nsstdlib.md") use this command
`arm-linux-gnueabi-gcc <Cfilename> <nsstdlib> -nostartfiles -nostdlib -nolibc -nodefaultlibs -o <outputFileName> -mcpu=arm7tdmi -T link.lds -Wno-builtin-declaration-mismatch`

`<nsstdlib>` This is the file that contains the nsstdlib code, IN THIS COMMIT IT IS CALLED syscalltest.c

`-Wno-builtin-declaration-mismatch` disables the warning about redefining the std lib functions

## Linker script
The linker script is just the basic script but with the .text section moved to 0x0. There is currently a bug that appears only on one machine I've tried where PHDR segment not covered by LOAD segment is displayed when compiling. I think this has to do with me removing all of the header data by moving the text section. Or at least I think I'm removing the header data, I'm not really sure. 

## Loading into Komodo
Once the file has been compiled to an ELF you can just LOAD it in KMD.
If all has gone well the start of the program should be at addr. 0x0 and so you can just reset Komodo and run. You can find the end of the main function by looking for BX R14 in the assembly view.  

##NSSTDlib
Included in the repo is the "not so standard library" as of this commit it is very bare bones, with putchar, print, and exit. 
Information on interacting with these functions can be found in the [nsstdlib.md](nsstdlib.md).  
To use the nsstdlib simply add `#include <nsstdlib>`

## Extra files
Also included are three test C programs along with their compiled versions, with the not so great naming scheme of \<cfile\>.c, \<cfile\>C. 

## Notes
This is a bit of a hodgepodge at the moment and so I'll attempt to refine it over time.  
When using optimisation flags, e.g. -O3, the compiler will probably remove the added SP change and so you may have to change it manually in KMD. Also the included versions are simple enough that at O3 it will all be reduced to a constant loaded into R0.

## TODO
I have not yet tried to have multiple C files that are then linked. I think this will work just fine, but it is not yet tested.  
SWI 3 and such are just like any other instruction and have a hex value for the opcode/operand. So can I use inline assembly with a defined constant for the SWIs and then for example move into R0 a pointer to some memory and then call swi 3. Does the compiler take into account inline assembly when tracking registers? May need to push/pop R0 before editing.
