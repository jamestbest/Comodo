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
		_stack_start = MAX_MEM_ADDR;
		_stack_end   = MAX_MEM_ADDR - STACK_SIZE;
		PROVIDE(__sstart = _stack_start);
	}
	
	.heap :
	{
		_heap_start = .;
		_heap_end   = _stack_end;
	}
}
