# Comodo
This is a repo with information/files for compiling C programs to an ELF format that the Komodo emulator can load and run

## C file
There are some fairly major caveats to this compilation, the most important being that you CANNOT use anything from the standard library as it will not be loaded.  
At the start of your program you should include `asm ("ldr r13, =$0x10000");` This will set the stack pointer to 10000 as I think that memory above a certain point is protected and will spit out 12345678 if you try to read to it.  

## Cross-compiler
In order to compile the C program you will need a cross-compiler for ARM  
`sudo apt-get install gcc-arm-linux-gnueabi`  

## Compiling
When compiling use this command  
`arm-linux-gnueabi-gcc <Cfilename> -nostartfiles -nostdlib -nolibc -nodefaultlibs -o <outputFileName> -mcpu=arm7tdmi -T link.lds`  

`-nostartfiles -nostdlib -nolibc -nodefaultlibs` This will disable all of the C libraries that are usually included, I don't think they're all needed I think some probably cover the rest but I'm not testing it.

`-mcpu=arm7tdmi` This is one of the supported architectures listed [here](https://studentnet.cs.manchester.ac.uk/resources/software/komodo/) under target systems

`-T linkerscript.lds` This will override the normal linker script. More info below

## Linker script
The linker script is just the basic script but with the .text section moved to 0x0. There is currently a bug that appears only on one machine I've tried where PHDR segment not covered by LOAD segment is displayed when compiling. I think this has to do with me removing all of the header data by moving the text section. Or at least I think I'm removing the header data, I'm not really sure. 

## Loading into Komodo
Once the file has been compiled to an ELF you can just LOAD it in KMD.
If all has gone well the start of the program should be at addr. 0x0 and so you can just reset Komodo and run. You can find the end of the main function by looking for BX R14 in the assembly view.  

## Extra files
Also included are three test C programs along with their compiled versions, with the not so great naming scheme of \<cfile\>.c, \<cfile\>C. 
