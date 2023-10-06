	ALIGN
	B main
main	MOV R1, #2
	MOV R2, #0
next	MUL r0, r1, r2
	SWI 4
	ADD r2, r2, #1
	CMP r2, #12
	BLE next
	SWI 2
