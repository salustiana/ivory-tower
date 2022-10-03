# function print to print a null
# terminated string.
# returns the ammount of characters printed
	
	.equ SYS_EXIT,		1
	.equ SYS_WRITE,		4
	.equ LINUX_SYSCALL,	0x80

	.equ STDOUT,		1

	.section .text

	.globl print
	.type print, @function
print:		# int print(const char *str);
	pushl %ebp
	movl %esp, %ebp

	pushl 8(%ebp)
	call strlen
	addl $4, %esp

	movl %eax, %edx
	movl $SYS_WRITE, %eax
	movl $STDOUT, %ebx
	movl 8(%ebp), %ecx
	int $LINUX_SYSCALL

	movl %edx, %eax		# return number of
				# printed chars
	movl %ebp, %esp
	popl %ebp
	ret
