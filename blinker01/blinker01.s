
.global reset
reset:
    mov.w #0x5A80,&0x120
    bis.b #0x41,&0x22

    bis.b #0x01,&0x21
    bic.b #0x40,&0x21

loop:
    xor.b #0x41,&0x21
    mov #0,r5
    mov #2,r6
loop0:
    sub #1,r5
    jnz loop0
    ;sub #1,r6
    ;jnz loop0

    jmp loop

.global hang
hang:
    jmp hang

