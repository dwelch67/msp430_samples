

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


sharp03.o : sharp03.c
	msp430-gcc $(COPS) -c sharp03.c -o sharp03.o


sharp03.elf : memmap startup.o reset.o sharp03.o
	msp430-ld -T memmap reset.o sharp03.o startup.o -o sharp03.elf

out.hex : sharp03.elf
	msp430-objdump -D sharp03.elf > sharp03.list
	msp430-objcopy -O ihex sharp03.elf out.hex




