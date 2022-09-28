# my first function
#

	.section .data
estring:
	.ascii "chars\n"
newline:
	.ascii "\n"

	.section .text
	.global _start
	.global prt
	.global exit

_start:
	pushl $6
	pushl $estring
	call prt

	pushl $5
	call sumar_2_prt

	movl %eax, %ebx
	movl $1, %eax
	int $0x80

	#pushl $0
	pushl %eax
	call exit

sumar_2_prt:	#	sumar_2_prt(int x) -> 2 * x

	# save caller stack frame
	# and create own stack frame
	pushl %ebp
	movl %esp, %ebp

	# allocate mem for local vars in stack
	subl $4, %esp

	# use local var to print ascii code for int
	movl 8(%ebp), %eax	# get param

	addl $2, %eax		# perform operation
	movl %eax, %ebx

	# prt(result)
	addl $48, %ebx		# offset to ascii char
	movl %ebx, -4(%ebp)
	pushl $1
	movl %ebp, %ebx
	subl $4, %ebx
	pushl %ebx
	call prt

	# prt('\n')
	pushl $1
	pushl $newline
	call prt

	# set return val
	movl -4(%ebp), %eax
	# reset caller stack frame
	movl %ebp, %esp
	popl %ebp
	ret

prt:	# 	prt(const char *str, int bytes)

	# save caller stack frame
	# and create own stack frame
	pushl %ebp
	movl %esp, %ebp

	# write syscall
	movl $4, %eax
	movl $1, %ebx
	movl 8(%ebp), %ecx	# get first param from stack
	movl 12(%ebp), %edx	# get second param
	int $0x80

	# reset caller stack frame
	movl %ebp, %esp
	popl %ebp
	ret

exit:	#	exit(int retval)

	# stack frame
	pushl %ebp
	movl %esp, %ebp

	# exit syscall
	movl $1, %eax
	movl 8(%ebp), %ebx
	int $0x80
