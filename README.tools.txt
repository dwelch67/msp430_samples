
Tools discussed/installed

1) naken430asm
2) llvm/clang
3) binutils (gnu assembler/linker)
4) mspgcc




1) naken430asm ------------------------------------------------------

If you want a cross platform assembler that does not take forever to
build, is not constantly being pulled this way and that by the opinions
of armies of developers, etc.  Miss the good old days of having .org
where you didnt have to have seven files, objects, linker and linker
script.  Realize that you dont need a C compiler to do embedded work.
Then naken430asm is for you.


http://www.mikekohn.net/micro/naken430asm_msp430_assembler.php

As of this writing this is the current release:

wget http://downloads.mikekohn.net/naken430asm/naken430asm-2011-09-21.tar.gz
tar xzvf naken430asm-2011-09-21.tar.gz
cd naken430asm-2011-09-21
./configure
make
make install

Many/most of my examples use naken430asm

2) llvm/clang -------------------------------------------------------

As of this writing llvm version 2.9 is the stable release, you can

apt-get install llvm clang

and get perhaps an older version or install from sources, it is very
easy just takes a long time

svn co http://llvm.org/svn/llvm-project/llvm/branches/release_29/ llvm29
cd llvm29
cd tools
svn co http://llvm.org/svn/llvm-project/cfe/branches/release_29/ clang
cd ..
./configure --enable-optimized --disable-doxygen --prefix=/llvm29
make
make install

if you have a multi core processor, for example a four core processor

make -j 4

will make the build go that much faster.  Only need the -j on the
make not the make install.

version 3.0 is in the process of being released, to install it:

svn co http://llvm.org/svn/llvm-project/llvm/branches/release_30/ llvm30
cd llvm30
cd tools
svn co http://llvm.org/svn/llvm-project/cfe/branches/release_30/ clang
cd ..
./configure --enable-optimized --disable-doxygen --prefix=/llvm30
make
make install


And there will be times you want to be cutting edge so that means:

svn co http://llvm.org/svn/llvm-project/llvm/trunk llvm
cd llvm
cd tools
svn co http://llvm.org/svn/llvm-project/cfe/trunk clang
cd ..
./configure --enable-optimized --disable-doxygen --prefix=/llvm
make
make install

Unfortunately the llvm make system is not perfect, it does have the
advantage that if you

make update

instead of svn update it updates both the main llvm tree and clang together
I like to run make update two or three times in a row just in case there
is an update in progress and you have only gotten some of the files

the negative side of the llvm make is that you have to

make clean

whenever there have been new files added to the repo.   You can try without
and ymmv, but if you get a build failure then try a make update and
make clean and try again.  if that fails then perhaps the last set of
changes is bad and they will figure that out and update more changes to
fix the build.  so wait for make update to show something then try again.

llvm is a great cross-compiler as it is always cross compiling.  It
does have the ability to link for the host it is on and sometimes the
host drives the default settings of the compiler (makes integers 64 or
32 bit for example).  It will compile to assembly language for any
of the supported targets though.  I use gnu binutils to take that
assembler the last few steps to a binary.  the mspgcc toolchain will
be covered in a bit, for now going to build binutils for msp430 from
sources.

3) binutils ---------------------------------------------------------

You may need a number of packages like texinfo and some others
it changes from time to time.  Certainly bison flex build-essential.

wget ftp://ftp.gnu.org/gnu/binutils/binutils-2.21.1a.tar.bz2
tar xjvf binutils-2.21.1a.tar.bz2
cd binutils-2.21.1
./configure --target=msp430 --prefix=/some/path/msp430
make
make install

Then when you want to use it
PATH=/some/path/msp430/bin:$PATH

msp430-as --version
GNU assembler (GNU Binutils) 2.21.1
Copyright 2011 Free Software Foundation, Inc.
This program is free software; you may redistribute it under the terms of
the GNU General Public License version 3 or later.
This program has absolutely no warranty.
This assembler was configured for a target of `msp430'.

4) mspgcc -----------------------------------------------------------

http://mspgcc.sf.net do all the manual work, download the stuff,
apply patches, build, pray.   mspgcc4 was SO much better than mspgcc
as a result I am probably going to abandon mspgcc all together and
take my chances with llvm (msp430 support being experimental).



