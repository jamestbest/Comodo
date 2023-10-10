ENTRY(_start)
STARTUP(krt0.o)

INPUT(nsstdlib)

STACK_SIZE   = 0x010000;
MAX_MEM_ADDR = 0x100000;

SECTIONS
{
	. = 0x0;
	
	.text (0x0) : { *(.text) }
	.data : { *(.data) }
	.bss  : { *(.bss ) }	
	.note.gnu.build-id : {}
	
	.stack :
	{

	}
	
	.heap :
	{
		HEAP_START = .;
		HEAP_END = MAX_MEM_ADDR;
	}
}
