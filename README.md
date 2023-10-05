# Comodo
This is a repo with information/files for compiling C programs to an ELF format that the Komodo emulator can load and run

## C file
There are some fairly major caveats to this compilation, the most important being that you CANNOT use anything from the standard library as it will not be loaded.  
At the start of your program you should include `asm ("ldr r13, =$0x10000");` This will set the stack pointer to 10000 as I think that memory above a certain point is protected and will spit out 12345678 if you try to read to it.  

## Cross-compiler
In order to compile the C program you will need a cross-compiler for ARM  
`sudo apt-get install arm-linux-gnueabi-gcc`  

## Compiling
When compiling use this command  
`arm-linux-gnueabi-gcc <Cfilename> -nostartfiles -nostdlib -nolibc -nodefaultlibs -o <outputFileName> -mcpu=arm7tdmi -T linkerscript.lds`  

`-nostartfiles -nostdlib -nolibc -nodefaultlibs` This will disable all of the C libraries that are usually included, I don't think they're all needed I think some probably cover the rest but I'm not testing it.

`-mcpu=arm7tdmi` This is one of the supported architectures listed [here](https://studentnet.cs.manchester.ac.uk/resources/software/komodo/) under target systems

`-T linkerscript.lds` This will override the normal linker script. More info below

## Linker script
Oh boy, well lets just say I "learnt" about linkerscripts and their structure by moving and removing parts, and so the included script SHOULD work, as its just the normal script with minor changes to the positioning of the sections in the ELF, mainly moving .text to start at 0x0, where KMD will start running.

## Loading into Komodo
Once the file has been compiled to an ELF you can just LOAD it in KMD, do not attempt to COMPILE it as it is already compiled.
If all has gone well the start of the program should be at addr. 0x0 and so you can just reset Komodo and run. You can find the end of the main function by looking for BX R14 in the assembly view.  
