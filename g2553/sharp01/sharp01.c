
void dummy ( unsigned short );

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


#define SPI_CS_ON  (P1OUT|= (1<<4))
#define SPI_CS_OFF (P1OUT&=~(1<<4))

#define SPI_CLK_ON  (P1OUT|= (1<<5))
#define SPI_CLK_OFF (P1OUT&=~(1<<5))

#define SPI_MOSI_ON  (P1OUT|= (1<<7))
#define SPI_MOSI_OFF (P1OUT&=~(1<<7))

//-------------------------------------------------------------------
void spi_delay ( void )
{
    //unsigned short ra;
    //for(ra=0;ra<100;ra++) dummy(ra);
}
//-------------------------------------------------------------------
void spi_write_byte ( unsigned char d )
{
    unsigned char ca;

    for(ca=0x80;ca;ca>>=1)
    {
        if(ca&d) SPI_MOSI_ON; else SPI_MOSI_OFF;
        spi_delay();
        SPI_CLK_ON;
        spi_delay();
        SPI_CLK_OFF;
        spi_delay();
    }
}
//-------------------------------------------------------------------
void spi_write_sharp_address ( unsigned char d )
{
    unsigned char ca;

    for(ca=0x01;;ca<<=1)
    {
        if(ca&d) SPI_MOSI_ON; else SPI_MOSI_OFF;
        spi_delay();
        SPI_CLK_ON;
        spi_delay();
        SPI_CLK_OFF;
        spi_delay();
        if(ca==0x80) break;
    }
}
//-------------------------------------------------------------------
void clear_screen ( void )
{
    SPI_CS_ON;
    spi_delay();
    spi_write_byte(0x20);
    spi_write_byte(0x00);
    spi_delay();
    SPI_CS_OFF;
}
//-------------------------------------------------------------------
void notmain ( void )
{
    unsigned short ra;
    unsigned short rb;
    unsigned char d;

    WDTCTL = 0x5A80;

    // use calibrated clock
    DCOCTL = 0x00;
    BCSCTL1 = CALBC1_16MHZ;
    DCOCTL = CALDCO_16MHZ;


    //P1.0 LCD Power Control
    //P1.3 LCD Enable
    //P1.4 SPI CS
    //P1.5 SPI CLK
    //P1.6   led2
    //p1.7 SPI MOSI

    //76543210
    //11111001
    //0x00xxxx
    //x1xx1xx1

    


    P1DIR|=0xF9;
    P1OUT&=~(0xB0);
    P1OUT|=0x49;

    TACTL = 0x02E0;


    clear_screen();

    for(d=0xF0;;d++)
    {
        for(ra=1;ra<=96;ra++)
        {
            P1OUT^=0x40;
            SPI_CS_ON;
            spi_delay();
            spi_write_byte(0x80);
            spi_write_sharp_address(ra&0xFF);
            for(rb=0;rb<12;rb++)
            {
                spi_write_byte(d);
            }
            spi_write_byte(0x00);
            spi_write_byte(0x00);
            spi_delay();
            SPI_CS_OFF;
        }
    }


    //while(1)
    //{

        ////0x80 then address then 12 bytes of data then 2 bytes of zeros
        ////

        //P1OUT^=0x40;

        //for(ra=0;ra<32;ra++)
        //{
            //while((TAR&0x8000)==0) continue;
            //while((TAR&0x8000)!=0) continue;
        //}

        //P1OUT^=0x40;

        //for(ra=0;ra<32;ra++)
        //{
            //while((TAR&0x8000)==0) continue;
            //while((TAR&0x8000)!=0) continue;
        //}
    //}
}
//-------------------------------------------------------------------
//-------------------------------------------------------------------
