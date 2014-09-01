

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

P3SEL   equ 0x022A

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

UCA0CTL1 equ 0x05C0
UCA0CTL0 equ 0x05C1
UCA0BR0  equ 0x05C6
UCA0BR1  equ 0x05C7
UCA0MCTL equ 0x05C8
UCA0STAT equ 0x05CA
UCA0RXBUF equ 0x05CC
UCA0TXBUF equ 0x05CE
UCA0ABCTL equ 0x05D0

UCA0IE equ 0x05DC
UCA0IFG equ 0x05DD

    org 0xFC00

reset:
    ; mov #0x0280,r1

    mov #0x5A80,&WDTCTL ; 0x5A00|WDTHOLD ;stop/hold wdt

    ; make p4.7 outputs
    bis.b #0x80,&P4DIR
    bic.b #0x80,&P4OUT

    bis.b #0x01,&UCA0CTL1
    mov.b #0x00,&UCA0CTL0
    mov.b #0x81,&UCA0CTL1
    mov.b #0x08,&UCA0BR0
    mov.b #0x00,&UCA0BR1
    mov.b #0x0C,&UCA0MCTL
    bis.b #0x18,&P3SEL
    bic.b #0x01,&UCA0CTL1

    mov #0,r9
loop:
    xor.b #0x80,&P4OUT

    add #1,r9
    and #7,r9
    bis #0x30,r9
    mov.b r9,&UCA0TXBUF

loop1a:
    bit.b #0x01,&UCA0STAT
    jnz loop1a

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
