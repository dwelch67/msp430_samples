

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

UCSCTL0 equ 0x0160
UCSCTL1 equ 0x0162
UCSCTL2 equ 0x0164
UCSCTL3 equ 0x0166
UCSCTL4 equ 0x0168
UCSCTL5 equ 0x016A
UCSCTL6 equ 0x016C
UCSCTL7 equ 0x016E
UCSCTL8 equ 0x0170

    org 0xFC00

reset:
    ; mov #0x0280,r1

    mov #0x5A80,&WDTCTL ; 0x5A00|WDTHOLD ;stop/hold wdt

    ; make p4.7 outputs
    bis.b #0x80,&P4DIR
    bic.b #0x80,&P4OUT

    mov #0x02E0,&TA0CTL ; smclk, /8, continuous,

loop:
    xor.b #0x80,&P4OUT

 loop0a:
     bit #0x8000,&TA0R
     jz loop0a
 loop0b:
     bit #0x8000,&TA0R
     jnz loop0b

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
