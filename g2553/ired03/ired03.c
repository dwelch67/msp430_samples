
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

//performing a min/max experiment
//min 0x3FD, max 0x500 1021 511us 1280 640us

//short high data bit
//min 0x336, max 0x460  822 411us  1120 560us

//long high data bit
//min 0xB94, max 0xCED  2964 1.482ms 3309 1.655ms


#define LOWMIN 1000
#define LOWMAX 1300
#define SHORTMIN 800
#define SHORTMAX 1200
#define LONGMIN  2900
#define LONGMAX  3400


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
void hexstring ( unsigned short d )
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
    uart_putc(0x0D);
    uart_putc(0x0A);
}
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
void notmain ( void )
{
    unsigned short sa,sb;
    unsigned short a,b,c;
    //unsigned short min, max;
    unsigned long code;
    //unsigned long lastcode;

    unsigned short data[32];


    WDTCTL = 0x5A80;

    // use calibrated clock
    DCOCTL = 0x00;
    BCSCTL1 = CALBC1_16MHZ;
    DCOCTL = CALDCO_16MHZ;

    uart_init();
    hexstring(0x1234);

    // make p1.0 and p1.6 outputs LEDs
    P1DIR |= 0x41;
    P1OUT &= ~0x41;
    // make p1.4 an input
    P1DIR&=~0x10;

    TACTL = 0x02E0;

//0x1D7E 0x2540   7550  9536

//max=0;
//min=0; min--;

    code=0;
    //lastcode=0;
    while(1)
    {
        while(P1IN&0x10) continue; //to LOW
        a=TAR;
        while(1) if(P1IN&0x10) break; //to HIGH
        b=TAR;
        c=b-a;
        if(c<(8540-1000)) continue;
        if(c>(8540+1000)) continue;
        sa=0;
        data[sa++]=c;
        for(sb=0;sb<14;sb++)
        {
            while(P1IN&0x10) continue; //to LOW
            a=TAR;
            data[sa++]=a-b;
            while(1) if(P1IN&0x10) break; //to HIGH
            b=TAR;
            data[sa++]=b-a;
            if(data[sa-1]>0x2000) break;

        }

        for(sb=0;sb<sa;sb++)
        {
            hexstrings(data[sb]);
        }
        uart_putc(0x0D);
        uart_putc(0x0A);
    }

}
//-------------------------------------------------------------------
//-------------------------------------------------------------------
