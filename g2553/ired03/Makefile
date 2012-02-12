

COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding 



AOPS = --warn --fatal-warnings

OOPS = -std-compile-opts -strip-debug -disable-simplify-libcalls

LOPS = -Wall -m32 -emit-llvm -disable-simplify-libcalls

HLOPS = -Wall -emit-llvm 

LLCOPS = -march=msp430

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


ired03.o : ired03.c
	msp430-gcc $(COPS) -c ired03.c -o ired03.o


ired03.elf : memmap startup.o reset.o ired03.o
	msp430-ld -T memmap reset.o ired03.o startup.o -o ired03.elf

out.hex : ired03.elf
	msp430-objdump -D ired03.elf > ired03.list
	msp430-objcopy -O ihex ired03.elf out.hex


