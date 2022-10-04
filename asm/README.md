# x86 assembly on an x86\_64 system

## standalone

```
$ as --32 prog.s -o prog.o
$ as --32 utils.s -o utils.o
$ ld -m elf_i386 prog.o utils.o -o prog
```

## linking with libraries

### install glibc32 from source

- get the source: (https://www.gnu.org/software/libc/sources.html)
```
git clone https://sourceware.org/git/glibc.git
cd glibc
git checkout release/2.36/master
```
- build:
```
$ mkdir build
$ cd build
$ ../configure --prefix=$HOME/glibc32-2.6 \
	--host=i686-linux-gnu --build=i686-linux-gnu \
	CC="gcc -m32" CXX="g++ -m32" CFLAGS="-O2 \
	-march=i686" CXXFLAGS="-O2 -march=i686"
$ make -j `nproc` install
```

### linking

```
$ as --32 prog.s -o prog.o
$ ld -m elf_i386 \
	-dynamic-linker ~/glibc32-2.6/lib/ld-linux.so.2 \
	-L ~/glibc32-2.6/lib prog.o -o prog -lc
```

## using gcc

- use `void _start()` as the entry point:
```
/* just exit */
void _start()
{
	asm ("\tmovl $1, %eax\n\tmovl $0, %ebx\n\tint $0x80\n");
}
```

- compile:
```
$ gcc -m32 -march=i386 -static -nostdlib prog.c -o prog
```

- or produce assembly source (this will produce prog.s):
```
$ gcc -m32 -march=i386 -static -nostdlib prog.c -S
```
