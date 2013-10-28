

COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding 

AOPS = --warn --fatal-warnings

gcc : out.hex

all : gcc clang
 

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


uart02.o : uart02.c
	msp430-gcc $(COPS) -c uart02.c -o uart02.o


uart02.elf : memmap startup.o reset.o uart02.o
	msp430-ld -T memmap reset.o uart02.o startup.o -o uart02.elf

out.hex : uart02.elf
	msp430-objdump -D uart02.elf > uart02.list
	msp430-objcopy -O ihex uart02.elf out.hex



OOPS = -std-compile-opts -strip-debug -disable-simplify-libcalls

xLOPS = -Wall -m32 -emit-llvm -disable-simplify-libcalls -target msp430-elf
LOPS = -Wall -m32 -emit-llvm -target msp430-elf

LLCOPS = -march=msp430 

clang : lout.opt.hex lout.hex

luart02.bc : uart02.c
	clang $(LOPS) -c uart02.c -o luart02.bc

luart02.o : luart02.bc
	#llc $(LLCOPS) -filetype=obj -o luart02.o luart02.bc
	llc $(LLCOPS) -o luart02.norm.s luart02.bc
	msp430-as $(AOPS) luart02.norm.s -o luart02.o

luart02.elf : memmap startup.o reset.o luart02.o
	msp430-ld -T memmap reset.o luart02.o startup.o -o luart02.elf

lout.hex : luart02.elf
	msp430-objdump -D luart02.elf > luart02.list
	msp430-objcopy -O ihex luart02.elf lout.hex

luart02.opt.bc : luart02.bc
	opt $(OOPS) luart02.bc -o luart02.opt.bc

luart02.opt.o : luart02.opt.bc
	#llc $(LLCOPS) -filetype=obj -o luart02.opt.o luart02.opt.bc
	llc $(LLCOPS) -o luart02.opt.s luart02.opt.bc
	msp430-as $(AOPS) luart02.opt.s -o luart02.opt.o
	

luart02.opt.elf : memmap startup.o reset.o luart02.opt.o
	msp430-ld -T memmap reset.o luart02.opt.o startup.o -o luart02.opt.elf

lout.opt.hex : luart02.opt.elf
	msp430-objdump -D luart02.opt.elf > luart02.opt.list
	msp430-objcopy -O ihex luart02.opt.elf lout.opt.hex


