

AOPS = --warn --fatal-warnings

OOPS = -std-compile-opts -strip-debug -disable-simplify-libcalls

LOPS = -Wall -m32 -emit-llvm 

HLOPS = -Wall -emit-llvm 

LLCOPS = -march=msp430


all : blinker03.opt.hex blinker03.norm.hex

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


blinker03.bc : blinker03.c
	clang $(LOPS) -c blinker03.c -o blinker03.bc

blinker03.opt.o : blinker03.bc
	opt $(OOPS) blinker03.bc -f -o blinker03.opt.bc
	llc $(LLCOPS) blinker03.opt.bc -o blinker03.opt.s
	msp430-as blinker03.opt.s -o blinker03.opt.o

blinker03.opt.elf : memmap startup.o reset.o blinker03.opt.o
	msp430-ld -T memmap reset.o blinker03.opt.o startup.o -o blinker03.opt.elf

blinker03.opt.hex : blinker03.opt.elf
	msp430-objdump -D blinker03.opt.elf > blinker03.opt.list
	msp430-objcopy -O ihex blinker03.opt.elf blinker03.opt.hex



blinker03.norm.o : blinker03.bc
	llc $(LLCOPS) blinker03.bc -o blinker03.norm.s
	msp430-as blinker03.norm.s -o blinker03.norm.o

blinker03.norm.elf : memmap startup.o reset.o blinker03.norm.o
	msp430-ld -T memmap reset.o blinker03.norm.o startup.o -o blinker03.norm.elf

blinker03.norm.hex : blinker03.norm.elf
	msp430-objdump -D blinker03.norm.elf > blinker03.norm.list
	msp430-objcopy -O ihex blinker03.norm.elf blinker03.norm.hex



