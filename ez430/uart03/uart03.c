
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


#define P1IN  (*((volatile unsigned char *)0x0020))
#define P1OUT (*((volatile unsigned char *)0x0021))
#define P1DIR (*((volatile unsigned char *)0x0022))

// 16MHz clock
// The timer is 16 bit
// set to divide by 1
// 16,000,000 / 155200 = 138.88889
#define TACCR0_VALUE 138

//-------------------------------------------------------------------
void uart_putc ( unsigned short c )
{
    unsigned short sa;
    unsigned short sb;
    unsigned short then,now;

    sa=c<<1;
    sa|=1<<9;
    sb=10;
    then=TAR;
    while(sb--)
    {
        if(sa&1) P1OUT|=1; else P1OUT&=(~1);
        sa>>=1;
        while(1)
        {
            now=TAR-then;
            if(now>TACCR0_VALUE) break;
        }
        then+=TACCR0_VALUE;
    }
}
//-------------------------------------------------------------------
void hexstring ( unsigned short d, unsigned short cr )
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
    if(cr)
    {
        uart_putc(0x0D);
        uart_putc(0x0A);
    }
    else
    {
        uart_putc(0x20);
    }
}
//-------------------------------------------------------------------
void notmain ( void )
{
    unsigned short /*sa,*/sb;
    //unsigned short start;
    unsigned short then; //,now;
    unsigned short bitin;
    //unsigned short log[32];

    WDTCTL = 0x5A80;

    // use calibrated clock
    DCOCTL = 0x00;
    BCSCTL1 = CALBC1_16MHZ;
    DCOCTL = CALDCO_16MHZ;

    // make p1.0 an output
    P1DIR |= 0x01;
    P1OUT |= 0x01;

    P1DIR &= ~0x02;


    BCSCTL2&=~0x06;
    TACTL = 0x0204;
    TACTL = 0x0220;

    hexstring(0x1234,1);
    hexstring(0x5678,1);

    while(1)
    {
        //sa=0;
        bitin=0;
        while(1) if((P1IN&2)==0) break;
        then=TAR;
        while(1)
        {
            if((TAR-then)>=(TACCR0_VALUE>>1)) break;
        }
        if(P1IN&2)
        {
            bitin>>=1;
            bitin|=1<<9;
        }
        else
        {
            bitin>>=1;
        }
        then+=(TACCR0_VALUE>>1);
        for(sb=0;sb<9;sb++)
        {
            while(1)
            {
                if((TAR-then)>=TACCR0_VALUE) break;
            }
            if(P1IN&2)
            {
                bitin>>=1;
                bitin|=1<<9;
            }
            else
            {
                bitin>>=1;
            }
            then+=TACCR0_VALUE;
        }
        hexstring(bitin,0);  hexstring(bitin>>1,1);
    }
}
//-------------------------------------------------------------------
//-------------------------------------------------------------------
