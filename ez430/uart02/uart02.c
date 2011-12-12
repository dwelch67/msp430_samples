
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


void uart_putc ( unsigned short c )
{
    unsigned short sa;
    unsigned short sb;

    sa=c<<1;
    sa|=1<<9;
    sb=10;
    while(sb--)
    {
        if(sa&1) P1OUT|=1; else P1OUT&=(~1);
        sa>>=1;
        while((TACCTL0&1)==0) continue;
        TACCTL0 &= (~1);
    }
}

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
    // set to divide by 1
    // 16,000,000 / 155200 = 138.88889

    BCSCTL2&=~0x06;
    TACTL = 0x0204;
    TACCR0 = 138;
    TACTL = 0x0210;

    for(sa=0;;sa++)
    {
        uart_putc((sa&7)|0x30);
    }

}

