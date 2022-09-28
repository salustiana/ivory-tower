src = ./prog.s
prog: ${src}
	as --32 ${src} -o prog.o
	ld -m elf_i386 prog.o -o prog
