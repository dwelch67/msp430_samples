
typedef unsigned long u32;


void dummy ( void );

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

#define P2IN  (*((volatile unsigned char *)0x0028))
#define P2OUT (*((volatile unsigned char *)0x0029))
#define P2DIR (*((volatile unsigned char *)0x002A))
#define P2REN (*((volatile unsigned char *)0x002F))
#define P2SEL (*((volatile unsigned char *)0x002E))
#define P2SEL2 (*((volatile unsigned char *)0x0042))

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
void delay_us ( unsigned int us )
{
    unsigned int ra;
    us<<=1;

    ra=TAR;
    while((TAR-ra)<us) continue;
}
//-------------------------------------------------------------------
#define IRQ  0
#define MISO 1
#define MOSI 2
#define SCK  3
#define CSN  4
#define CE   5

//-------------------------------------------------------------------
void nordic_csn_set( void )
{
    P2OUT|=(1<<CSN);
}
//-------------------------------------------------------------------
void nordic_csn_clr( void )
{
    P2OUT&=~(1<<CSN);
}
//-------------------------------------------------------------------
void nordic_ce_set( void )
{
    P2OUT|=(1<<CE);
}
//-------------------------------------------------------------------
void nordic_ce_clr( void )
{
    P2OUT&=~(1<<CE);
}
//-------------------------------------------------------------------
void nordic_sck_set( void )
{
    P2OUT|=(1<<SCK);
}
//-------------------------------------------------------------------
void nordic_sck_clr( void )
{
    P2OUT&=~(1<<SCK);
}
//-------------------------------------------------------------------
void nordic_mosi_set( void )
{
    P2OUT|=(1<<MOSI);
}
//-------------------------------------------------------------------
void nordic_mosi_clr( void )
{
    P2OUT&=~(1<<MOSI);
}
//-------------------------------------------------------------------
unsigned int nordic_miso ( void )
{
    return(P2IN&(1<<MISO));
}
//-------------------------------------------------------------------
unsigned int nordic_read_register ( unsigned int reg )
{
    unsigned int ra;
    unsigned int rb;

    reg&=0x1F;
    reg|=0x00; //read register
    nordic_csn_clr();
    dummy();

    rb=0;
    for(ra=0x80;ra;ra>>=1)
    {
        if(ra&reg) nordic_mosi_set();
        else       nordic_mosi_clr();
        dummy();
        nordic_sck_set();
        dummy();
        rb<<=1;
        if(nordic_miso()) rb|=1;
        nordic_sck_clr();
        dummy();
    }
    for(ra=0;ra<8;ra++)
    {
        nordic_sck_set();
        dummy();
        rb<<=1;
        if(nordic_miso()) rb|=1;
        nordic_sck_clr();
        dummy();
    }
    nordic_csn_set();
    dummy();
    return(rb);
}
//-------------------------------------------------------------------
unsigned int nordic_write_register ( unsigned int reg, unsigned int data )
{
    unsigned int ra;
    unsigned int rb;

    reg&=0x1F;
    reg|=0x20; //write register
    data&=0xFF;

    nordic_csn_clr();
    dummy();
    rb=0;
    for(ra=0x80;ra;ra>>=1)
    {
        if(ra&reg) nordic_mosi_set();
        else       nordic_mosi_clr();
        dummy();
        nordic_sck_set();
        dummy();
        rb<<=1;
        if(nordic_miso()) rb|=1;
        nordic_sck_clr();
        dummy();
    }
    for(ra=0x80;ra;ra>>=1)
    {
        if(ra&data) nordic_mosi_set();
        else       nordic_mosi_clr();
        dummy();
        nordic_sck_set();
        dummy();
        rb<<=1;
        if(nordic_miso()) rb|=1;
        nordic_sck_clr();
        dummy();
    }
    nordic_csn_set();
    dummy();
    return(rb);
}
//-------------------------------------------------------------------
u32 nordic_read_payload ( void )
{
    u32 data;

    unsigned int ra;
    unsigned int reg;

    reg=0x61;

    nordic_csn_clr();
    dummy();

    for(ra=0x80;ra;ra>>=1)
    {
        if(ra&reg) nordic_mosi_set();
        else       nordic_mosi_clr();
        dummy();
        nordic_sck_set();
        dummy();
        nordic_sck_clr();
        dummy();
    }
    data=0;
    for(ra=0;ra<32;ra++)
    {
        nordic_sck_set();
        dummy();
        data<<=1;
        if(nordic_miso()) data|=1;
        nordic_sck_clr();
        dummy();
    }
    nordic_csn_set();
    dummy();
    return(data);
}
//-------------------------------------------------------------------
void nordic_init ( void )
{
    unsigned int ra;

    nordic_ce_clr();
    nordic_csn_set();
    nordic_sck_clr();

    ra=nordic_read_register(0x00); //config register
    ra|=0x02; //PWR_UP
    ra=nordic_write_register(0x00,ra);
    //wait at least 4.5ms
    delay_us(4500);
}
//-------------------------------------------------------------------
void notmain ( void )
{
    u32 payload;
    unsigned int ra;
    unsigned int rb;

    WDTCTL = 0x5A80;

    // use calibrated clock
    DCOCTL = 0x00;
    BCSCTL1 = CALBC1_16MHZ;
    DCOCTL = CALDCO_16MHZ;

    uart_init();
    hexstring(0x1234);

    TACTL = 0x02E0;

    // make p1.0 and p1.6 outputs LEDs
    P1DIR |= 0x41;
    P1OUT &= ~0x41;

    //disable pullup/down resistor
    P2REN&=~((1<<IRQ)|(1<<MISO)|(1<<MOSI)|(1<<SCK)|(1<<CSN)|(1<<CE));

    P2SEL&=~((1<<IRQ)|(1<<MISO)|(1<<MOSI)|(1<<SCK)|(1<<CSN)|(1<<CE));
    P2SEL2&=~((1<<IRQ)|(1<<MISO)|(1<<MOSI)|(1<<SCK)|(1<<CSN)|(1<<CE));

    //make MISO and IRQ inputs
    P2DIR&=~((1<<IRQ)|(1<<MISO));
    //make the others outputs
    P2DIR|=(1<<MOSI)|(1<<SCK)|(1<<CSN)|(1<<CE);

    nordic_ce_clr();
    nordic_csn_set();
    nordic_sck_clr();

    nordic_init();
    for(rb=0;rb<0x10;rb++)
    {

        ra=nordic_read_register(rb);
        hexstring(ra);
    }

    //set transfer size to 4
    nordic_write_register(0x11,4);
    //set rx mode
    ra=nordic_read_register(0x00); //config register
    ra|=0x01; //PRIM_RX
    ra=nordic_write_register(0x00,ra);
    //enable receivier
    nordic_ce_set();

    while(1)
    {
        //ra=nordic_read_register(0x07);
        rb=nordic_read_register(0x17);
        if(rb&1)
        {
        }
        else
        {
            payload=nordic_read_payload();
            hexstrings(rb); hexstrings((payload>>16)&0xFFFF); hexstring((payload>>0)&0xFFFF);
        }
    }

    //make them all inputs, dont drive
    P2DIR&=~((1<<IRQ)|(1<<MISO)|(1<<MOSI)|(1<<SCK)|(1<<CSN)|(1<<CE));
    P2REN&=~((1<<IRQ)|(1<<MISO)|(1<<MOSI)|(1<<SCK)|(1<<CSN)|(1<<CE));
}
//-------------------------------------------------------------------
//-------------------------------------------------------------------
