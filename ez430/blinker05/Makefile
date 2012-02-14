

COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding 

AOPS = --warn --fatal-warnings

all : out.hex

clean :
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.bc
	rm -f *.hex
	rm -f *.norm.s
	rm -f *.opt.s


startup.o : startup.s
	msp430-as startup.s -o startup.o

reset.o : reset.s
	msp430-as reset.s -o reset.o


blinker05.o : blinker05.c
	msp430-gcc $(COPS) -c blinker05.c -o blinker05.o


blinker05.elf : memmap startup.o reset.o blinker05.o
	msp430-ld -T memmap reset.o blinker05.o startup.o -o blinker05.elf

out.hex : blinker05.elf
	msp430-objdump -D blinker05.elf > blinker05.list
	msp430-objcopy -O ihex blinker05.elf out.hex


