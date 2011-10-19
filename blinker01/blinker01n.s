

    ;This version is written for naken430asm.
    ;http://www.mikekohn.net/micro/naken430asm_msp430_assembler.php
    ;naken430asm -o filename.hex filename.s
    ;mspdebug takes hex files as well as elfs.

org 0xFC00

reset:
    mov #0x5A80,&0x120
    bis #0x0041,&0x22

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

hang:
    jmp hang

org 0xFFE0
    dw hang
    dw hang
    dw hang
    dw hang
    dw hang
    dw hang
    dw hang
    dw hang
    dw hang
    dw hang
    dw hang
    dw hang
    dw hang
    dw hang
    dw hang
    dw reset
