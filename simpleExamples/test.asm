main
    mov r0, #12

    ldr r1, =0x3b9ac9fc

    str r0, [r1]

    ldr r2, [r1]

    swi 2