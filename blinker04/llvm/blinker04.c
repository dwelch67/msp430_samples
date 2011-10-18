

#define WDTCTL     (*((volatile unsigned short *)0x0120))

#define CALBC1_1MHZ (*((volatile unsigned char *)0x10FF))
#define CALDCO_1MHZ (*((volatile unsigned char *)0x10FE))

#define DCOCTL  (*((volatile unsigned char *)0x56))

#define BCSCTL1 (*((volatile unsigned char *)0x57))
#define BCSCTL2 (*((volatile unsigned char *)0x58))

#define TACTL   (*((volatile unsigned short *)0x0160))
#define TAR     (*((volatile unsigned short *)0x0170))
#define TACCR0  (*((volatile unsigned short *)0x0172))
#define TAIV    (*((volatile unsigned short *)0x012E))
#define TACCTL0 (*((volatile unsigned short *)0x0162))

#define P1OUT (*((volatile unsigned char *)0x0021))
#define P1DIR (*((volatile unsigned char *)0x0022))

void bis_r2 ( unsigned short );

void notmain ( void )
{
    WDTCTL = 0x5A80;

    // use calibrated clock
    DCOCTL = 0x00;
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;

    // make p1.0 and p1.6 outputs
    P1DIR |= 0x41;
    P1OUT &= (~0x41);
    P1OUT |= 0x40;

    // 1MHz is 1000000 clocks per second
    // 1000000 = 0xF4240
    // The timers are 16 bit
    // Using a divide by 8 in BCSCTL2 gives
    // 125000 (0x1E848) clocks in a second
    // Using a divide by 8 in the timer gives
    // 15625 (0x3D09) timer ticks per second.

    // If both divisors are by 8, and we set
    // TACCR0 to 0x3D08 and set for count up mode
    // then, theory, we can measure seconds.

    BCSCTL2|=0x06;

    TACCTL0 = 0x0010;
    TACCR0 = 0x3D08;
    TACTL = 0x02D0;

    bis_r2(0x0018);
    //cpu should actually be off and not enter this loop
    while(1) continue;
}

void ccifg_int_handler ( void )
{
    P1OUT ^= 0x41;
}
