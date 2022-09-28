	.section .data

	.section .text
	.globl _start

_start:
	# divide eax by ebx
	movl $32, %eax
	movl $4, %ebx
	movl $0, %edx	# edx must be cleared
	idivl %ebx

	movl %eax, %ebx
	movl $1, %eax
	int $0x80
