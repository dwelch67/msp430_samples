

AOPS = --warn --fatal-warnings

OOPS = -std-compile-opts -strip-debug -disable-simplify-libcalls

LOPS = -Wall -m32 -emit-llvm 

HLOPS = -Wall -emit-llvm 

LLCOPS = -march=msp430


all : blinker02.opt.hex blinker02.norm.hex

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


blinker02.bc : blinker02.c
	clang $(LOPS) -c blinker02.c -o blinker02.bc

blinker02.opt.o : blinker02.bc
	opt $(OOPS) blinker02.bc -f -o blinker02.opt.bc
	llc $(LLCOPS) blinker02.opt.bc -o blinker02.opt.s
	msp430-as blinker02.opt.s -o blinker02.opt.o

blinker02.opt.elf : memmap startup.o reset.o blinker02.opt.o
	msp430-ld -T memmap reset.o blinker02.opt.o startup.o -o blinker02.opt.elf

blinker02.opt.hex : blinker02.opt.elf
	msp430-objdump -D blinker02.opt.elf > blinker02.opt.list
	msp430-objcopy -O ihex blinker02.opt.elf blinker02.opt.hex



blinker02.norm.o : blinker02.bc
	llc $(LLCOPS) blinker02.bc -o blinker02.norm.s
	msp430-as blinker02.norm.s -o blinker02.norm.o

blinker02.norm.elf : memmap startup.o reset.o blinker02.norm.o
	msp430-ld -T memmap reset.o blinker02.norm.o startup.o -o blinker02.norm.elf

blinker02.norm.hex : blinker02.norm.elf
	msp430-objdump -D blinker02.norm.elf > blinker02.norm.list
	msp430-objcopy -O ihex blinker02.norm.elf blinker02.norm.hex



