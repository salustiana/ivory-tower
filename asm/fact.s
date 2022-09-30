# FUNCTION:
#	int fact(int n);
#	return factorial of n
#
#	int fact_rec(int n);
#	return factorial of n, calculated recursively
#

	.section .text
	.globl _start
	.globl fact
	.globl fact_rec

	.type fact, @function

_start:
	pushl $5
	call fact_rec
	movl %eax, %ebx
	jmp exit

exit:
	movl $1, %eax
	int $0x80

fact:
	pushl %ebp		# save caller's base pointer
	movl %esp, %ebp		# create our stack frame

	cmpl $1, 8(%ebp)	# return 0 if n <= 1
	movl $1, %eax
	jle fact_end

	movl 8(%ebp), %eax	# store n in %eax
	movl %eax, %ebx		# and copy it to %ebx

fact_loop:
	decl %ebx		# decrement %ebx
	cmpl $1, %ebx		# if %ebx is 1, end loop
	je fact_end

	imull %ebx, %eax	# multiply %eax by %ebx
	jmp fact_loop		# loop

fact_end:
	movl %ebp, %esp		# restore stack and
	popl %ebp		# caller's base pointer

	ret			# %eax already holds retval

# recursive version
	.type fact_rec, @function
fact_rec:
	pushl %ebp		# save caller's base pointer
	movl %esp, %ebp		# create our stack frame

	cmpl $1, 8(%ebp)	# return 1 if n <= 1
	movl $1, %eax
	jle fact_rec_end

	movl 8(%ebp), %ebx	# call fact_rec with n-1
	decl %ebx
	pushl %ebx
	call fact_rec
	addl $4, %esp		# reset stack

	movl 8(%ebp), %ebx
	imull %ebx, %eax	# multiply fact(n-1) by n

	jmp fact_rec_end	# return result

fact_rec_end:
	movl %ebp, %esp		# restore stack and
	popl %ebp		# caller's base pointer

	ret			# %eax already holds retval
