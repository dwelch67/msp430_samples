

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

UCA0CTL0    equ 0x060
UCA0CTL1    equ 0x061
UCA0BR0     equ 0x062
UCA0BR1     equ 0x063
UCA0MCTL    equ 0x064
UCA0STAT    equ 0x065
UCA0RXBUF   equ 0x066
UCA0TXBUF   equ 0x067
UCA0ABCTL   equ 0x05D
UCA0IRTCTL  equ 0x05E
UCA0IRRCTL  equ 0x05F
IE2         equ 0x001
IFG2        equ 0x003

P3REN equ 0x10
P3OUT equ 0x19
P3DIR equ 0x1A
P3SEL equ 0x1B

    org 0xFC00

reset:
    mov #0x0280,r1

    mov #0x5A80,&WDTCTL ; 0x5A00|WDTHOLD

    ; use calibrated clock
    clr.b &DCOCTL
    mov.b &CALBC1_16MHZ,&BCSCTL1
    mov.b &CALDCO_16MHZ,&DCOCTL

    bic.b #0x06,&BCSCTL2

  mov.b #0x00,&UCA0CTL0
  mov.b #0xC0,&UCA0CTL1

  mov.b #0x08,&UCA0BR0
  mov.b #0x00,&UCA0BR1
  mov.b #0xB1,&UCA0MCTL

  ;mov.b #0x8A,&UCA0BR0
  ;mov.b #0x00,&UCA0BR1
  ;mov.b #0x0E,&UCA0MCTL

  ;mov.b #0xA0,&UCA0BR0
  ;mov.b #0x01,&UCA0BR1
  ;mov.b #0x0C,&UCA0MCTL

  mov.b #0x00,&UCA0STAT
  mov.b #0x00,&UCA0ABCTL
  mov.b #0x00,&IE2
;  mov.b #0x40,&UCA0CTL1
  bic.b #0x30,&P3REN
  ;bis.b #0x10,&P3DIR
  bis.b #0x30,&P3SEL




    ;jmp over

txbusy:
    bit.b #0x01,&UCA0STAT
    jnz txbusy

    mov.b #0x55,&UCA0TXBUF

txbusyb:
    bit.b #0x01,&UCA0STAT
    jnz txbusyb

    mov.b #0x56,&UCA0TXBUF


    jmp txbusy


over:


    ; make p1.0 and p1.6 outputs
    bis.b #0xFF,&P1DIR


    mov #0x02E0,&TACTL

loop:

    bic.b #0xFF,&P1OUT
    mov.b #0x55,&UCA0TXBUF

    mov #32,r9
loop0a:
    bit #0x8000,&TAR
    jz loop0a
loop0b:
    bit #0x8000,&TAR
    jnz loop0b
    sub #1,r9
    jnz loop0a


    bis.b #0xFF,&P1OUT
    mov.b #0x56,&UCA0TXBUF


    mov #32,r9
loop1a:
    bit #0x8000,&TAR
    jz loop1a
loop1b:
    bit #0x8000,&TAR
    jnz loop1b
    sub #1,r9
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
