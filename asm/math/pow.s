# FUNCTION:
#	int power(int b, uint p);
#	raise b to the p power and return result
#

	.section .text
	.globl power

	.type power, @function
power:
	pushl %ebp		# save caller's base pointer
	movl %esp, %ebp		# create our stack frame

	cmpl $0, 12(%ebp)	# return 0 if power <= 0
	jle ret_zero

	movl 8(%ebp), %eax	# store base in %eax
	movl 12(%ebp), %ebx	# store power in %ebx

power_loop:
	cmpl $1, %ebx		# if power is 1, end loop
	je power_end

	imull 8(%ebp), %eax	# multiply %eax by base
	decl %ebx		# decrement power
	jmp power_loop		# loop

ret_zero:
	movl $0, %eax
	jmp power_end

power_end:
	movl %ebp, %esp		# restore stack and
	popl %ebp		# caller's base pointer

	ret			# %eax already holds retval
