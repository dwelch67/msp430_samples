

blinker01.elf : memmap startup.o blinker01.o 
	msp430-ld -T memmap startup.o blinker01.o -o blinker01.elf
	msp430-objdump -D blinker01.elf > blinker01.list

startup.o : startup.s
	msp430-as startup.s -o startup.o

blinker01.o : blinker01.s
	msp430-as blinker01.s -o blinker01.o


clean :
	rm -f *.o
	rm -f *.list
	rm -f *.elf

