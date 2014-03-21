

    ;This version is written for naken430asm.
    ;http://www.mikekohn.net/micro/naken430asm_msp430_assembler.php
    ;naken430asm -o filename.hex filename.s
    ;mspdebug takes hex files as well as elfs.

WDTCTL equ 0x015C

TA0CTL   equ 0x0340
TA0CCTL0 equ 0x0342
TA0R     equ 0x0350
TA0CCR0  equ 0x0352
TA0IV    equ 0x036E


P1OUT   equ 0x0202
P1DIR   equ 0x0204

P4OUT   equ 0x0223
P4DIR   equ 0x0225

    org 0xFC00

reset:
    ; mov #0x0280,r1

    mov #0x5A80,&WDTCTL ; 0x5A00|WDTHOLD

    ; make p4.7 outputs
    bis.b #0x80,&P4DIR
    bic.b #0x80,&P4OUT

    mov #0x02E0,&TA0CTL

loop:

    xor.b #0x80,&P4OUT

    mov #6,r9
loop0a:
    bit #0x8000,&TA0R
    jz loop0a
loop0b:
    bit #0x8000,&TA0R
    jnz loop0b
    sub #1,r9
    jnz loop0a

    jmp loop

hang:
    jmp hang

    org 0xFFE0
    dw hang  ; 0xFFE0
    dw hang  ; 0xFFE2
    dw hang  ; 0xFFE4
    dw hang  ; 0xFFE6
    dw hang  ; 0xFFE8
    dw hang  ; 0xFFEA
    dw hang  ; 0xFFEC
    dw hang  ; 0xFFEE
    dw hang  ; 0xFFF0
    dw hang  ; 0xFFF2
    dw hang  ; 0xFFF4
    dw hang  ; 0xFFF6
    dw hang  ; 0xFFF8
    dw hang  ; 0xFFFA
    dw hang  ; 0xFFFC
    dw reset ; 0xFFFE
