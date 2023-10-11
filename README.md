# Comodo
This is a repo with information/files for compiling C programs to an ELF format that the Komodo emulator can load and run

# [Comodo's gone dynamic! 🥳](./NSSTDLib.md#Dynamic-memory-allocation)

## C file
You will not be able to use anything from the C standard library as it will not be loaded. All the SWIs have changed or been removed by kmd so nothing would work anyway.  
In place of the C standard library is the NSSTDlib documented [here](NSSTDLib.md)

## Cross-compiler
In order to compile the C program you will need a cross-compiler for ARM  
`sudo apt-get install gcc-arm-linux-gnueabi`

## Compiling
To compile a program without the NSSTDlib run the `runNS.sh` script with "bash <path2runNS.sh>"  
To compile a program with the NSSTDlib run the `runWS.sh` script with "bash <path2runWS.sh>" 

Both scripts require input in the form of `<inputfile0> <inputfile1> ... <inputfilen> -o <outputfile>`  
All includes and libraries will be included in the compilation, to view the specifics of this you can open and edit the bash scripts.

## Loading into Komodo
Once the file has been compiled to an ELF you can just LOAD it in KMD.
If all has gone well the start of the program should be at addr. 0x0 and so you can just reset Komodo and run. You can find the end of the main function by looking for BX R14 in the assembly view.  

## NSSTDlib
Included in the repo is the "not so standard library". 
Information on interacting with these functions can be found in the [nsstdlib.md](NSSTDLib.md).  
To use the nsstdlib include the headerfile at the top of the C file and include the file in the compilation process, described [here](#Compiling)

## Linker script
There are two linkerscripts provided, both are used to format the ELF to place the starting function (_start) at the 0x0 so that komodo can be run without moving the program counter. They both also provide an address for the start of the stack i.e. MAX_MEM_ADDR.

LinkC.lds is used when linking with the standard library and will also provide information on the format and position of the heap as well as having the nsstdlib as an input.  
The only other difference between the files is that LinkC uses krt0 as the startup file whereas linkS uses krt1, details below.

## krt0, krt1
Also included are two files that are used as the actual entry point of your program, they define a function _start that in both cases sets up the stack pointer and call main. krt0 is used when calling a file that includes the NSSTDlib and so also calls heapCreate to setup the heap with the given start and end memory addresses. It also calls reset after main which will print out the error code of main aswell as call swi 2 to end kmd. The instruction that follows is `mov r15, #0` which mean a program that has run successfully does not need to be reset, you can just use run again.

krt1 is used when not linking the nsstdlib and will setup the stack, call main, and then exit komodo with swi 2.  

The krt0/1 object files are what is used in linking, but the arm files are also included.  

## Using GCC optimisations
GCC optimisations should work, the krt0/1 and NSSTDlib were excluded from optimisation as from testing it caused some issues with inline assembly, although more testing is to be completed.

## Extra files
Included are some examples that do not use the NSSTDlib along with some that do. They have both their C source file and compiled elf.

## BTS
Within the bash scripts you will find the setup flags for the cross compiler below are some explanations/information about why they are included  

Within RunNS.sh which is the file to run without the standard library
`arm-linux-gnueabi-gcc ${inputfiles[@]} -o $outputfile -nostartfiles -nostdlib -nolibc -nodefaultlibs -mcpu=arm7tdmi -T ${dir}/linkS.lds -L${dir}/Libs`
 
This passes the input files and the name for the output file  

`-nostartfiles -nostdlib -nolibc -nodefaultlibs` This will disable all of the C libraries that are usually included, I don't think they're all needed I think some probably cover the rest but I'm not testing it.

`-mcpu=arm7tdmi` This is one of the supported architectures listed [here](https://studentnet.cs.manchester.ac.uk/resources/software/komodo/) under target systems

`-T ${dir}/linkS.lds` This will override the normal linker script. The new script is linkS, it describes the layout of the generated elf.

`-L${dir}/Libs` This is adds the path to krt1.o, the startup file for files without the NSSTDlib.

Within RunWS.sh which is the file to run with the standard library

`arm-linux-gnueabi-gcc ${inputfiles[@]} -o $outputfile -nostartfiles -nostdlib -nolibc -nodefaultlibs -mcpu=arm7tdmi -T ${dir}/linkC.lds -I${dir}/Includes -L${dir}/Libs -Wno-builtin-declaration-mismatch`

`-I${dir}/Includes` This adds an include directory where the NSSTDlib's header file is stored

`-L${dir}/Libs` This adds a lib directory to where the NSSTDlib compiled library can be found

`-Wno-builtin-declaration-mismatch` disables the warning about redefining the std lib functions

## Disclaimer
This project is not intended be used to generate any arm assembly that is submitted for coursework, the difference between compiler generated and person generated assembly is easy enough to distingish anyway but just to say that's not why I started this project.  
This project includes a couple things that I leant about during me working on it; linkerscripts, ELF, startup files (crt0), bash files. It is also where the majority of my C coding practice has come from, so there is bound to be some terrible practices and code that makes no sense, but here is ya warning.

## TODO
I have not yet tried to have multiple C files that are then linked. I think this will work just fine, but it is not yet tested.    
