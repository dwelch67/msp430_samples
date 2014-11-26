
//#define DEBUG_UART

const unsigned char numfont[50]=
{
0xF, // ####
0x9, // #..#
0x9, // #..#
0x9, // #..#
0xF, // ####
     //
0x1, // ...#
0x1, // ...#
0x1, // ...#
0x1, // ...#
0x1, // ...#
     //
0xF, // ####
0x1, // ...#
0xF, // ####
0x8, // #...
0xF, // ####
     //
0xF, // ####
0x1, // ...#
0xF, // ####
0x1, // ...#
0xF, // ####
     //
0x9, // #..#
0x9, // #..#
0xF, // ####
0x1, // ...#
0x1, // ...#
     //
0xF, // ####
0x8, // #...
0xF, // ####
0x1, // ...#
0xF, // ####
     //
0xF, // ####
0x8, // #...
0xF, // ####
0x9, // #..#
0xF, // ####
     //
0xF, // ####
0x1, // ...#
0x1, // ...#
0x1, // ...#
0x1, // ...#
     //
0xF, // ####
0x9, // #..#
0xF, // ####
0x9, // #..#
0xF, // ####
     //
0xF, // ####
0x9, // #..#
0xF, // ####
0x1, // ...#
0x1, // ...#
     //
};

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

unsigned char xstring[16];
unsigned short lasttime;

unsigned short valid;

//-------------------------------------------------------------------
void uart_init ( void )
{
    BCSCTL2 &=~0x06;

    UCA0CTL0 = 0x00;
    UCA0CTL1 = 0xC0;
    UCA0BR0  = 104;
    UCA0BR1  = 0;
    UCA0MCTL = 0x31;

    UCA0STAT  = 0x00;
    UCA0ABCTL = 0x00;
    IE2       = 0x00;

    P1SEL  |= 0x06;
    P1SEL2 |= 0x06;
}
//-------------------------------------------------------------------
void uart_putc ( unsigned char c )
{
    //while(UCA0STAT&0x01) continue;
    while(1)
    {
        if(IFG2&2)
        {
            UCA0TXBUF=c;
            break;
        }
    }
}
unsigned char uart_getc ( void )
{
    while(1)
    {
        if(IFG2&1)
        {
            return(UCA0RXBUF);
        }
    }
}

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

void show_time ( void )
{
    unsigned short nowtime;
    unsigned short ra;
    unsigned short rc;
    unsigned short ry;
    unsigned char ca,cb,cc;
    unsigned char base[4];

    nowtime =xstring[0]&0xF;
    nowtime<<=4;
    nowtime|=xstring[1]&0xF;
    nowtime<<=4;
    nowtime|=xstring[2]&0xF;
    nowtime<<=4;
    nowtime|=xstring[3]&0xF;
    if(nowtime==lasttime) return;

    lasttime=nowtime;

    for(ra=0;ra<4;ra++)
    {
        base[ra]=((xstring[ra]&0xF)<<2)+(xstring[ra]&0xF);
    }


    for(ry=0,ra=4;ra<44;ra++,ry++)
    {
        //P1OUT^=0x40;

        SPI_CS_ON;
        spi_delay();
        spi_write_byte(0x80);
        spi_write_sharp_address(ra&0xFF);

        spi_write_byte(0xFF);

        rc=base[0]+(ry>>3);
        ca=numfont[rc];

        for(cb=8;cb;cb>>=1)
        {
            if(cb&ca) cc=0x00; else cc=0xFF;
            spi_write_byte(cc);
        }

        spi_write_byte(0xFF);

        rc=base[1]+(ry>>3);
        ca=numfont[rc];

        for(cb=8;cb;cb>>=1)
        {
            if(cb&ca) cc=0x00; else cc=0xFF;
            spi_write_byte(cc);
        }

        spi_write_byte(0xFF);
        spi_write_byte(0xFF);

        spi_write_byte(0x00);
        spi_write_byte(0x00);
        spi_delay();
        SPI_CS_OFF;
    }

    for(ry=0,ra=44+8;ra<44+48;ra++,ry++)
    {

        SPI_CS_ON;
        spi_delay();
        spi_write_byte(0x80);
        spi_write_sharp_address(ra&0xFF);

        spi_write_byte(0xFF);

        rc=base[2]+(ry>>3);
        ca=numfont[rc];

        for(cb=8;cb;cb>>=1)
        {
            if(cb&ca) cc=0x00; else cc=0xFF;
            spi_write_byte(cc);
        }

        spi_write_byte(0xFF);

        rc=base[3]+(ry>>3);
        ca=numfont[rc];

        for(cb=8;cb;cb>>=1)
        {
            if(cb&ca) cc=0x00; else cc=0xFF;
            spi_write_byte(cc);
        }

        spi_write_byte(0xFF);
        spi_write_byte(0xFF);

        spi_write_byte(0x00);
        spi_write_byte(0x00);
        spi_delay();
        SPI_CS_OFF;
    }
}
//-------------------------------------------------------------------
void notmain ( void )
{
    unsigned short ra;
    unsigned short rb;
    unsigned short rc;

    unsigned short state,off,ncom;

    WDTCTL = 0x5A80;

    // use calibrated clock
    DCOCTL = 0x00;
    BCSCTL1 = CALBC1_16MHZ;
    DCOCTL = CALDCO_16MHZ;

    uart_init();


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

                P1OUT^=0x40;

    clear_screen();


    lasttime=0;


    //toggle_seconds=0;
    valid=0;
    state=0;
    off=0;
    ncom=0;
//$GPRMC,001412.799,V,,,,,0.00,0.00,060180,,,N*43
//$GPRMC,054033.00,V,
    while(1)
    {
        ra=uart_getc();
        //uart_putc(ra);
        switch(state)
        {
            case 0:
            {
                if(ra==0x0A) state++;
                break;
            }
            case 1:
            {
                if(ra=='$') state++;
                else state=0;
                break;
            }
            case 2:
            {
                if(ra=='G') state++;
                else state=0;
                break;
            }
            case 3:
            {
                if(ra=='P') state++;
                else state=0;
                break;
            }
            case 4:
            {
                if(ra=='R') state++;
                else state=0;
                break;
            }
            case 5:
            {
                if(ra=='M') state++;
                else state=0;
                break;
            }
            case 6:
            {
                if(ra=='C') state++;
                else state=0;
                break;
            }
            case 7:
            {
                off=0;
                ncom=0;
                if(ra==',') state++;
                else state=0;
                break;
            }
            case 8:
            {
                if((ncom)&&(ra==','))
                {
                    valid=0;
                    for(rb=0;rb<off;rb++)
                    {
                        if(xstring[rb]==',')
                        {
                            if(xstring[rb+1]=='A') valid=1;
                            break;
                        }
                    }
                    //012345678901
                    //002329.799,V
                    if(off>7)
                    {
#ifdef DEBUG_UART
                        for(rb=0;rb<off;rb++)
                        {
                            uart_putc(xstring[rb]);
                        }
                        uart_putc(0x20);
                        uart_putc(0x30+valid);
                        uart_putc(0x20);
#endif

                        //if((valid)&&(locked))
                        if(1)
                        {

                            //002329.799,V

                            //need to manipulate the hours to adjust for
                            //time zone

                            rb=xstring[0]&0xF;
                            rc=xstring[1]&0xF;
                            //first digit times 10 plus second is hours gmt
                            //1010
                            rb=/*rb*10*/(rb<<3)+(rb<<1); //times 10
                            rb+=rc;
                            //adjust for 24 hour clock
                            if(rb>12) rb-=12;
                            //======================================
                            ra=5; //time zone adjustment
                            if(rb<=ra) rb+=12;
                            rb-=ra;
                            //======================================
                            //divide by 10 first digit is either 1 or not
                            if(rb>9)
                            {
                                xstring[0]='1';
                                rb-=10;
                            }
                            else
                            {
                                xstring[0]='0';
                            }
                            rb&=0xF;
                            xstring[1]=0x30+rb;
                            xstring[4]=0;
                        }
                        else
                        {
                            xstring[0]=0x31;
                            xstring[1]=0x39;
                            xstring[2]=0x37;
                            xstring[3]=0x30|(valid);
                            xstring[4]=0;
                        }
                    }
                    else
                    {
                        xstring[0]=0x31;
                        xstring[1]=0x39;
                        xstring[2]=0x39;
                        xstring[3]=0x39;
                        xstring[4]=0;
                    }
#ifdef DEBUG_UART
                    for(ra=0;ra<4;ra++)
                    {
                        uart_putc(xstring[ra]);
                    }
                    uart_putc(0x0D);
                    uart_putc(0x0A);
#endif
                    show_time();
                    off=0;
                    state++;
                }
                else
                {
                    if(ra==',') ncom++;
                    if(off<16)
                    {
                        xstring[off++]=ra;
                    }
                }
                break;
            }
            case 9:
            {
                state=0;
                //if(zstring[off]==0)
                //{
                    //state=0;
                //}
                //else
                //{
                    //uart_putc(zstring[off++]);
                //}
                break;
            }

        }
    }






}
//-------------------------------------------------------------------
//-------------------------------------------------------------------
