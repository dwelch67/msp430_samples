

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

    TACCTL0 = 0x0010;
    TACCR0 = 0xFFFF;
    //TACTL = 0x02D0; //clock divide/8
    //TACTL = 0x0290; //clock divide/4
    TACTL = 0x0250; //clock divide/2
    //TACTL = 0x0210; //clock divide/1

  //  bis_r2(0x0018);
    //cpu should actually be off and not enter this loop
    while(1) continue;
}

