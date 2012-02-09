

    ;This version is written for naken430asm.
    ;http://www.mikekohn.net/micro/naken430asm_msp430_assembler.php
    ;naken430asm -o filename.hex filename.s
    ;mspdebug takes hex files as well as elfs.

WDTCTL equ 0x0120


CALBC1_1MHZ equ 0x10FF
CALDCO_1MHZ equ 0x10FE

CALBC1_8MHZ equ 0x10FD
CALDCO_8MHZ equ 0x10FC

CALBC1_12MHZ equ 0x10FB
CALDCO_12MHZ equ 0x10FA

CALBC1_16MHZ equ 0x10F9
CALDCO_16MHZ equ 0x10F8

DCOCTL  equ 0x56
BCSCTL1 equ 0x57
BCSCTL2 equ 0x58

TACTL   equ 0x0160
TAR     equ 0x0170
TACCR0  equ 0x0172
TAIV    equ 0x012E
TACCTL0 equ 0x0162


P1OUT   equ 0x0021
P1DIR   equ 0x0022


    org 0xFC00

reset:
    mov #0x0280,r1

    mov #0x5A80,&WDTCTL ; 0x5A00|WDTHOLD

    ; use calibrated clock
    clr.b &DCOCTL
    mov.b &CALBC1_16MHZ,&BCSCTL1
    mov.b &CALDCO_16MHZ,&DCOCTL

    ; make p1.0 and p1.6 outputs
    bis.b #0x41,&P1DIR
    bis.b #0x01,&P1OUT
    bic.b #0x40,&P1OUT

    ; 16MHZ
    ; SMCLK divisor /8
    ; timer divisor /8
    ; 16000000 / 64 = 250000
    ; 16 bit timer roll over 16 times
    ; 250000 / (16*65536) = 0.2384...
    ; invert that and 4.194 seconds.
    ; the led should change every 4 seconds

    bis.b #0x06,&BCSCTL2
    mov #0x02E0,&TACTL

loop:

    xor.b #0x41,&P1OUT

    mov #16,r9
loop0a:
    bit #0x8000,&TAR
    jz loop0a
loop0b:
    bit #0x8000,&TAR
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
