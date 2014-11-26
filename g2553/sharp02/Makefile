

COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding 

AOPS = --warn --fatal-warnings

gcc : out.hex

all : gcc 
 

clean :
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.bc
	rm -f *.hex
	rm -f *.norm.s
	rm -f *.opt.s


startup.o : startup.s
	msp430-as $(AOPS) startup.s -o startup.o

reset.o : reset.s
	msp430-as $(AOPS) reset.s -o reset.o


sharp02.o : sharp02.c
	msp430-gcc $(COPS) -c sharp02.c -o sharp02.o


sharp02.elf : memmap startup.o reset.o sharp02.o
	msp430-ld -T memmap reset.o sharp02.o startup.o -o sharp02.elf

out.hex : sharp02.elf
	msp430-objdump -D sharp02.elf > sharp02.list
	msp430-objcopy -O ihex sharp02.elf out.hex




