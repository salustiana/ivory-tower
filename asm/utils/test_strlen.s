	.section .data
estring:
	.ascii "hola amigos\0"

	.section .text
	.globl _start
_start:
	pushl $estring
	call strlen

	movl %eax, %ebx
	# exit
	movl $1, %eax
	int $0x80
