ENTRY(_start)
STARTUP(krt1.o)

MAX_MEM_ADDR = 0x100000;

SECTIONS
{
	. = 0x0;
	
	.text (0x0) : { *(.text) }
	.data : { *(.data) }
	.bss  : { *(.bss ) }	
	.note.gnu.build-id : {}
}
