

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
    bis.b #0x01,&P1DIR
    bis.b #0x01,&P1OUT

    ; 16MHz is 16000000 clocks per second
    ; The timers are 16 bit
    ; Using a divide by 8 in BCSCTL2 gives
    ; 2000000 clocks in a second
    ; Using a divide by 8 in the timer gives
    ; 250000 timer ticks per second.

    ; If both divisors are by 8, and we set
    ; TACCR0 to 62500, then counting down four times is one second

    bis.b #0x06,&BCSCTL2
    mov #0x02C4,&TACTL
    mov #62499,&TACCR0
    mov #0x02D0,&TACTL

loop:
    xor.b #0x01,&P1OUT
    mov #4,r6
loop0:
    bit.w #0x0001,&TACCTL0
    jz loop0
    bic.w #0x0001,&TACCTL0
    sub #1,r6
    jnz loop0
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
