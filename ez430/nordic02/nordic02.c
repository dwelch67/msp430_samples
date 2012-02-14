
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

#define P1IN  (*((volatile unsigned char *)0x0020))
#define P1OUT (*((volatile unsigned char *)0x0021))
#define P1DIR (*((volatile unsigned char *)0x0022))
#define P1REN (*((volatile unsigned char *)0x0027))
#define P1SEL (*((volatile unsigned char *)0x0026))

//-------------------------------------------------------------------
void delay_us ( unsigned int us )
{
    unsigned int ra;
    us<<=1;

    ra=TAR;
    while((TAR-ra)<us) continue;
}
//-------------------------------------------------------------------
#define MISO 1
#define MOSI 2
#define SCK  3
#define CSN  4
#define CE   5

//-------------------------------------------------------------------
void nordic_csn_set( void )
{
    P1OUT|=(1<<CSN);
}
//-------------------------------------------------------------------
void nordic_csn_clr( void )
{
    P1OUT&=~(1<<CSN);
}
//-------------------------------------------------------------------
void nordic_ce_set( void )
{
    P1OUT|=(1<<CE);
}
//-------------------------------------------------------------------
void nordic_ce_clr( void )
{
    P1OUT&=~(1<<CE);
}
//-------------------------------------------------------------------
void nordic_sck_set( void )
{
    P1OUT|=(1<<SCK);
}
//-------------------------------------------------------------------
void nordic_sck_clr( void )
{
    P1OUT&=~(1<<SCK);
}
//-------------------------------------------------------------------
void nordic_mosi_set( void )
{
    P1OUT|=(1<<MOSI);
}
//-------------------------------------------------------------------
void nordic_mosi_clr( void )
{
    P1OUT&=~(1<<MOSI);
}
//-------------------------------------------------------------------
unsigned int nordic_miso ( void )
{
    return(P1IN&(1<<MISO));
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
unsigned int nordic_read_payload ( void )
{
    unsigned int data;

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
    for(ra=0;ra<16;ra++)
    {
        nordic_sck_set();
        dummy();
        data<<=1;
        if(nordic_miso()) data|=1;
        nordic_sck_clr();
        dummy();
    }
    for(ra=0;ra<16;ra++)
    {
        nordic_sck_set();
        dummy();
        nordic_sck_clr();
        dummy();
    }
    nordic_csn_set();
    dummy();
    return(data);
}
//-------------------------------------------------------------------
void notmain ( void )
{
    unsigned int payload;
    unsigned int ra;
    unsigned int rb;

    WDTCTL = 0x5A80;

    // use calibrated clock
    DCOCTL = 0x00;
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;

    TACTL = 0x02E0;

    // make p1.0 outputs LEDs
    P1DIR |= 0x01;
    P1OUT &= ~0x01;

    //disable pullup/down resistor
    P1REN&=~((1<<MISO)|(1<<MOSI)|(1<<SCK)|(1<<CSN)|(1<<CE));
    P1SEL&=~((1<<MISO)|(1<<MOSI)|(1<<SCK)|(1<<CSN)|(1<<CE));
    //make MISO an input
    P1DIR&=~((1<<MISO));
    //make the others outputs
    P1DIR|=(1<<MOSI)|(1<<SCK)|(1<<CSN)|(1<<CE);
    nordic_ce_clr();
    nordic_csn_set();
    nordic_sck_clr();
    ra=nordic_read_register(0x00); //config register
    ra|=0x02; //PWR_UP
    ra=nordic_write_register(0x00,ra);
    //wait at least 4.5ms
    delay_us(4500);
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
            //if((payload&0x0100)==0x0000) //top
            //if((payload&0x0200)==0x0000) //bottom
            //if((payload&0x0400)==0x0000) //left
            //if((payload&0x0800)==0x0000) //right
            //if((payload&0x1000)==0x0000) //middle
            if((payload&0x0C00)==0x0000) //left+right
            {
                P1OUT |= 0x1;
                ra=TAR;
                while((TAR-ra)<20000) continue;
                P1OUT &= ~0x1;
            }
        }
    }

    //make them all inputs, dont drive
    P1DIR&=~((1<<MISO)|(1<<MOSI)|(1<<SCK)|(1<<CSN)|(1<<CE));
    P1REN&=~((1<<MISO)|(1<<MOSI)|(1<<SCK)|(1<<CSN)|(1<<CE));
}
//-------------------------------------------------------------------
//-------------------------------------------------------------------
