

.equ WDTCTL,0x0120
.equ P1OUT,0x0021
.equ P1DIR,0x0022


.global reset
reset:
    mov.w #0x5A80,&WDTCTL
    bis.b #0x0041,&P1DIR

loop:
    bis.b #0x0001,&P1OUT
    bic.b #0x0040,&P1OUT
    mov.w #0,r5
    mov.w #2,r6
loop0:
    sub #1,r5
    jnz loop0
    ;sub #1,r6
    ;jnz loop0

    bic.b #0x0001,&P1OUT
    bis.b #0x0040,&P1OUT

    mov.w #0,r5
    mov.w #2,r6
loop1:
    sub #1,r5
    jnz loop1
    ;sub #1,r6
    ;jnz loop1

    jmp loop

.global hang
hang:
    jmp hang

