
.text

.global _start

_start:
	ldr r13, =MAX_MEM_ADDR

	ldr r0, =HEAP_START
	ldr r1, =MAX_MEM_ADDR
	bl heapCreate

	bl main

	bl reset

.data
.global MAX_MEM_ADDR
.global HEAP_START
.global HEAP_END
