	.section .data
fmtstr:
	.ascii "this is an int: %d\n\0"

	.section .text
	.globl _start
_start:
	pushl $124
	pushl $fmtstr
	call printf

	pushl $0
	call exit
