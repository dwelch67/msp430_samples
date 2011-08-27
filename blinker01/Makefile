
all : blinker01.elf blinker01a.elf

blinker01.elf : memmap startup.o blinker01.o 
	msp430-ld -T memmap startup.o blinker01.o -o blinker01.elf
	msp430-objdump -D blinker01.elf > blinker01.list


blinker01.o : blinker01.s
	msp430-as blinker01.s -o blinker01.o


blinker01a.elf : memmap startup.o blinker01a.o 
	msp430-ld -T memmap startup.o blinker01a.o -o blinker01a.elf
	msp430-objdump -D blinker01a.elf > blinker01a.list


blinker01a.o : blinker01a.s
	msp430-as blinker01a.s -o blinker01a.o



startup.o : startup.s
	msp430-as startup.s -o startup.o


blinker01b.hex : blinker01b.s 
	naken430asm -o blinker01b.hex blinker01b.s


clean :
	rm -f *.o
	rm -f *.list
	rm -f *.elf
	rm -f *.hex

