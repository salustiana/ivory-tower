	.section .data
estring:
	.ascii "hola amigos\n\0"

	.section .text
	.globl _start
_start:
	pushl $estring
	call print

	# exit
	movl $1, %eax
	movl $0, %ebx
	int $0x80
