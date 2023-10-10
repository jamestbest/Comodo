.text

.global _start

_start:
	ldr r13, =MAX_MEM_ADDR

	bl main

	swi 2
	mov r15, #0


.data
.global MAX_MEM_ADDR
