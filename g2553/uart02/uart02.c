
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
#define TACCTL0 (*((volatile unsigned short *)0x0162))

#define UCA0CTL0    (*((volatile unsigned char *)0x060 ))
#define UCA0CTL1    (*((volatile unsigned char *)0x061 ))
#define UCA0BR0     (*((volatile unsigned char *)0x062 ))
#define UCA0BR1     (*((volatile unsigned char *)0x063 ))
#define UCA0MCTL    (*((volatile unsigned char *)0x064 ))
#define UCA0STAT    (*((volatile unsigned char *)0x065 ))
#define UCA0RXBUF   (*((volatile unsigned char *)0x066 ))
#define UCA0TXBUF   (*((volatile unsigned char *)0x067 ))
#define UCA0ABCTL   (*((volatile unsigned char *)0x05D ))
#define UCA0IRTCTL  (*((volatile unsigned char *)0x05E ))
#define UCA0IRRCTL  (*((volatile unsigned char *)0x05F ))
#define IE2         (*((volatile unsigned char *)0x001 ))
#define IFG2        (*((volatile unsigned char *)0x003 ))



#define P1IN  (*((volatile unsigned char *)0x0020))
#define P1OUT (*((volatile unsigned char *)0x0021))
#define P1DIR (*((volatile unsigned char *)0x0022))
#define P1REN (*((volatile unsigned char *)0x0027))
#define P1SEL (*((volatile unsigned char *)0x0026))
#define P1SEL2 (*((volatile unsigned char *)0x0041))

//-------------------------------------------------------------------
void uart_init ( void )
{
    BCSCTL2 &=~0x06;

    UCA0CTL0 = 0x00;
    UCA0CTL1 = 0xC0;
    UCA0BR0  = 0x08;
    UCA0BR1  = 0x00;
    UCA0MCTL = 0xB1;

    UCA0STAT  = 0x00;
    UCA0ABCTL = 0x00;
    IE2       = 0x00;

    P1SEL  |= 0x06;
    P1SEL2 |= 0x06;
}
//-------------------------------------------------------------------
void uart_putc ( unsigned char c )
{
    while(UCA0STAT&0x01) continue;
    UCA0TXBUF=c;
}
//-------------------------------------------------------------------
//-------------------------------------------------------------------
void hexstrings ( unsigned short d )
{
    //unsigned short ra;
    unsigned short rb;
    unsigned short rc;

    rb=16;
    while(1)
    {
        rb-=4;
        rc=(d>>rb)&0xF;
        if(rc>9) rc+=0x37; else rc+=0x30;
        uart_putc(rc);
        if(rb==0) break;
    }
    uart_putc(0x20);
}
//-------------------------------------------------------------------
void hexstring ( unsigned short d )
{
    hexstrings(d);
    uart_putc(0x0D);
    uart_putc(0x0A);
}
//-------------------------------------------------------------------
void notmain ( void )
{
    unsigned short ra;

    WDTCTL = 0x5A80;

    // use calibrated clock
    DCOCTL = 0x00;
    BCSCTL1 = CALBC1_16MHZ;
    DCOCTL = CALDCO_16MHZ;

    uart_init();
    hexstring(0x1234);

    P1DIR|=0x41;
    P1OUT&=~(0x01);
    P1OUT|=0x40;

    TACTL = 0x02E0;

    while(1)
    {
        P1OUT^=0x41;
        UCA0TXBUF = 0x55;

        for(ra=0;ra<32;ra++)
        {
            while((TAR&0x8000)==0) continue;
            while((TAR&0x8000)!=0) continue;
        }

        P1OUT^=0x41;
        UCA0TXBUF = 0x56;

        for(ra=0;ra<32;ra++)
        {
            while((TAR&0x8000)==0) continue;
            while((TAR&0x8000)!=0) continue;
        }
    }
}
//-------------------------------------------------------------------
//-------------------------------------------------------------------
