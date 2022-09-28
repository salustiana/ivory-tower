	.section .data
palabra:
	.ascii "peperon\n"

	.section .text
	.globl _start

_start:
	pushq $8
	pushq $palabra
	call prt
	pushq $0
	call exit
