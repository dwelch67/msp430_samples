

AOPS = --warn --fatal-warnings

OOPS = -std-compile-opts -strip-debug -disable-simplify-libcalls

LOPS = -Wall -m32 -emit-llvm 

HLOPS = -Wall -emit-llvm 

LLCOPS = -march=msp430


all : blinker04.opt.hex blinker04.norm.hex

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


blinker04.bc : blinker04.c
	clang $(LOPS) -c blinker04.c -o blinker04.bc

blinker04.opt.o : blinker04.bc
	opt $(OOPS) blinker04.bc -f -o blinker04.opt.bc
	llc $(LLCOPS) blinker04.opt.bc -o blinker04.opt.s
	msp430-as blinker04.opt.s -o blinker04.opt.o

blinker04.opt.elf : memmap startup.o reset.o blinker04.opt.o
	msp430-ld -T memmap reset.o blinker04.opt.o startup.o -o blinker04.opt.elf

blinker04.opt.hex : blinker04.opt.elf
	msp430-objdump -D blinker04.opt.elf > blinker04.opt.list
	msp430-objcopy -O ihex blinker04.opt.elf blinker04.opt.hex



blinker04.norm.o : blinker04.bc
	llc $(LLCOPS) blinker04.bc -o blinker04.norm.s
	msp430-as blinker04.norm.s -o blinker04.norm.o

blinker04.norm.elf : memmap startup.o reset.o blinker04.norm.o
	msp430-ld -T memmap reset.o blinker04.norm.o startup.o -o blinker04.norm.elf

blinker04.norm.hex : blinker04.norm.elf
	msp430-objdump -D blinker04.norm.elf > blinker04.norm.list
	msp430-objcopy -O ihex blinker04.norm.elf blinker04.norm.hex



