

AOPS = --warn --fatal-warnings

OOPS = -std-compile-opts -strip-debug -disable-simplify-libcalls

LOPS = -Wall -m32 -emit-llvm 

HLOPS = -Wall -emit-llvm 

LLCOPS = -march=msp430


all : uart02.opt.hex uart02.norm.hex

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


uart02.bc : uart02.c
	clang $(LOPS) -c uart02.c -o uart02.bc

uart02.opt.o : uart02.bc
	opt $(OOPS) uart02.bc -f -o uart02.opt.bc
	llc $(LLCOPS) uart02.opt.bc -o uart02.opt.s
	msp430-as uart02.opt.s -o uart02.opt.o

uart02.opt.elf : memmap startup.o reset.o uart02.opt.o
	msp430-ld -T memmap reset.o uart02.opt.o startup.o -o uart02.opt.elf

uart02.opt.hex : uart02.opt.elf
	msp430-objdump -D uart02.opt.elf > uart02.opt.list
	msp430-objcopy -O ihex uart02.opt.elf uart02.opt.hex



uart02.norm.o : uart02.bc
	llc $(LLCOPS) uart02.bc -o uart02.norm.s
	msp430-as uart02.norm.s -o uart02.norm.o

uart02.norm.elf : memmap startup.o reset.o uart02.norm.o
	msp430-ld -T memmap reset.o uart02.norm.o startup.o -o uart02.norm.elf

uart02.norm.hex : uart02.norm.elf
	msp430-objdump -D uart02.norm.elf > uart02.norm.list
	msp430-objcopy -O ihex uart02.norm.elf uart02.norm.hex



