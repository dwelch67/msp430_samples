
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
    unsigned short sb;
    unsigned short a,b,c;
  //  unsigned short min, max;
    unsigned long code;
    //unsigned long lastcode;

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

//using this receiver...
//http://www.sparkfun.com/products/10266

//And decoding what resembles the JVC protocol at least as defined here
//http://www.sbprojects.com/knowledge/ir/jvc.php

//idle is high, there is a unique/long low start pulse
//using a scope the pulse looks to be 9.2ms
//there is a long high pulse after the start pulse
//high part of start pattern 4.5ms for a code and 2.24 for a repeat
//with a repeat you get a 590us pulse to return and then a gap
//codes also get the 590us pulse but are followed by more pulses
//with this protocol after this first low pulse you will see either long
//or short highs with the same size low pulse.  The high is one of two
//lengths, assume or arbirtrarily define one length to be a one and the
//other a zero.  There are 32 bits once decoded.

//so 32 low pulses, two transistions per plus the intro takes 4
//transitions 36 transitions total for a full code, a repeat if supported
//by the remote is 4 transitions.

//I have three remotes handy that generate this protocol, one for a
//canon camcorder.  One for a toshiba cd player?  RM-A210 and one is
//for a pioneer cd player CU-PDD34

//canon repeats by repeating the whole command not a shortened pulse
//for most commands only generates 5 pulses per press, rewind and
//such will continue to repeat

//pioneer appears to repeat whole command not a repeat pulse, and repeats
//any of the buttons forever.

//start pulse
//pioneer remote 0x41D0 16848  8.4ms
//toshiba remote 0x4580 17792  8.8ms

//8.6ms is 17200
//17200 - 1720 = 15480
//17200 + 1720 = 18920

//second pulse and repeat pulse
//pioneer  0x41xx 0x20Ax 8532 4.2ms
//toshiba  0x45xx 0x224x 8768 4.4ms
//toshiba  0x45xx 0x11xx 4532 2.2ms

//call it 4.3ms 8600
//8600 - 860 = 7740
//8600 + 860 = 9460
//repeat call 2.15ms 4300
//4300 - 430 = 3870
//4300 + 430 = 4730

//wondering if the difference is the IR frequency, guess not
//toshiba 0x45xx 0x22xx  0x67Ex total
//pioneer 0x41xx 0x20xx  0x624x total
//canon   0x45xx 0x22xx  0x67Cx total

//low pulse between codes pioneer is around 0x470 other two are a little
//higher  0x470 = 1136   568 us

//performing a min/max experiment across all 33 low pulses
//min 0x3FD, max 0x500 1021 511us 1280 640us

//short high data bit
//min 0x336, max 0x460  822 411us  1120 560us

//long high data bit
//min 0xB94, max 0xCED  2964 1.482ms 3309 1.655ms

//max=0;
//min=0; min--;

    code=0;
    //lastcode=0;
    while(1)
    {
        while(P1IN&0x10) continue;
        a=TAR;
        while(1) if(P1IN&0x10) break;
        b=TAR;
        c=b-a;
        if(c<(17200-1720)) continue;
        if(c>(17200+1720)) continue;
        while(P1IN&0x10) continue;
        a=TAR;
        c=a-b;
        if(c<(4300-430)) continue;
        //if(c>(4300+430)) continue;
        //if(c<(8600-860)) continue;
        if(c>(8600+860)) continue;
        if(c<4300+430)
        {
            //repeat
            while(1) if(P1IN&0x10) break;
            //code=lastcode;
            code=0x12345678;
            P1OUT |= 0x01; //blink led
            hexstrings((code>>16)&0xFFFF);
            hexstring((code>>0)&0xFFFF);
            P1OUT &= ~0x01;
            continue;
        }
        if(c<(8600-860)) continue;
        while(1) if(P1IN&0x10) break;
        b=TAR;
        c=b-a; //short
        //if(c<min) min=c;
        //if(c>max) max=c;
        if(c<LOWMIN) continue;
        if(c>LOWMAX) continue;
        code=0;
        for(sb=0;sb<32;sb++)
        {

            while(P1IN&0x10) continue;
            a=TAR;
            c=a-b; //short/long high
            ////if(c<2000)
            //if(c>2000)
            //{
                //if(c<min) min=c;
                //if(c>max) max=c;
            //}
            if(c<SHORTMIN) continue;
            if(c>LONGMAX) continue;
            code<<=1;
            if(c<SHORTMAX)
            {
                //code|=0;
            }
            else
            {
                if(c>LONGMIN)
                {
                    code|=1;
                }
                else
                {
                    continue;
                }
            }
            while(1) if(P1IN&0x10) break;
            b=TAR;
            c=b-a; //shortlow
            //if(c<min) min=c;
            //if(c>max) max=c;
            if(c<LOWMIN) continue;
            if(c>LOWMAX) continue;
        }
        //lastcode=code;
        P1OUT |= 0x41; //blink led
        hexstrings((code>>16)&0xFFFF);
        hexstring((code>>0)&0xFFFF);
        P1OUT &= ~0x41;
    }

}
//-------------------------------------------------------------------
//-------------------------------------------------------------------
