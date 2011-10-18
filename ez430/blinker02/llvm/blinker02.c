
#define WDTCTL     (*((volatile unsigned short *)0x0120))

#define CALBC1_1MHZ (*((volatile unsigned char *)0x10FF))
#define CALDCO_1MHZ (*((volatile unsigned char *)0x10FE))
#define CALBC1_8MHZ (*((volatile unsigned char *)0x10FD))
#define CALDCO_8MHZ (*((volatile unsigned char *)0x10FC))
#define CALBC1_12MHZ (*((volatile unsigned char *)0x10FB))
#define CALDCO_12MHZ (*((volatile unsigned char *)0x10FA))
#define CALBC1_16MHZ (*((volatile unsigned char *)0x10F9))
#define CALDCO_16MHZ (*((volatile unsigned char *)0x10F8))

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

void notmain ( void )
{
    unsigned short sa;

    WDTCTL = 0x5A80;

    // use calibrated clock
    DCOCTL = 0x00;
    BCSCTL1 = CALBC1_16MHZ;
    DCOCTL = CALDCO_16MHZ;

    // make p1.0 an output
    P1DIR |= 0x01;
    P1OUT |= 0x01;

    // 16MHz is 16000000 clocks per second
    // The timers are 16 bit
    // Using a divide by 8 in BCSCTL2 gives
    // 2000000 clocks in a second
    // Using a divide by 8 in the timer gives
    // 250000 timer ticks per second.

    // If both divisors are by 8, and we set
    // TACCR0 to 62500, then counting down four times is one second

    BCSCTL2|=0x06;
    TACTL = 0x02C4;
    TACCR0 = 62499;
    TACTL = 0x02D0;

    while(1)
    {
        P1OUT ^= 0x01;
        for(sa=0;sa<4;sa++)
        {
            while((TACCTL0&1)==0) continue;
            TACCTL0 &= (~1);
        }
    }
}

