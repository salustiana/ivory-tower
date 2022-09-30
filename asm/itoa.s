# FUNCTION:
#	void itoa(char *out, int n);
#	fills out string with ascii
#	representation of n

	.section .data
parola:
	.string "hola"
	.equ STRLEN, 4

	.section .bss
	.equ MAX_DIGITS, 1024
	.lcomm itoa_ret, MAX_DIGITS

	.section .text
	.globl _start
	.globl itoa

	.type itoa, @function

_start:
	#pushl $STRLEN
	#pushl $parola
	#call reverse

	# write syscall
	#movl $4, %eax
	#movl $1, %ebx
	#movl $parola, %ecx
	#movl $STRLEN, %edx
	#int $0x80

	pushl $-132
	pushl $parola
	call itoa

	# write syscall
	movl $4, %eax
	movl $1, %ebx
	movl $parola, %ecx
	movl $4, %edx
	int $0x80

	jmp exit

exit:
	movl $1, %eax
	int $0x80


itoa:	# void itoa(char *out, int n);
# registers:
#	%ecx - holds string pointer
#	%eax - holds copy of n to work with
#	%ebx - hold number 10
#	%edi - holds string index

	pushl %ebp		# save caller's base pointer
	movl %esp, %ebp		# create our stack frame

	# load registers
	movl 8(%ebp), %ecx
	movl 12(%ebp), %eax
	movl $10, %ebx
	movl $0, %edi

	cmpl $0, %eax
	jge itoa_loop
	neg %eax		# work with positive number

itoa_loop:
	cmpl $0, %eax		# while n != 0
	je itoa_loop_end

	# get n % 10
	movl $0, %edx		# edx must be cleared to
				# store the remainder
				# %eax holds n
	divl %ebx		# divide by 10

	# now %eax /= 10 and %edx = %eax % 10

	movl %edx, (%ecx,%edi,1)# store result in string
	addl $'0', (%ecx,%edi,1)# converted to ascii

	# %eax already holds the value we need
	incl %edi
	jmp itoa_loop

itoa_loop_end:
	cmpl $0, 12(%ebp)	# if n is negative
	jge reverse_itoa	# we must add a minus sign

	movl $'-', (%ecx,%edi,1)# add minus sign
	incl %edi		# account for new length

reverse_itoa:
	pushl %edi		# pass string length
	pushl %ecx		# pass string pointer
	call reverse
	# no need to reset the stack since we are returning now
	jmp func_ret		# return once string is reversed


reverse:	# void reverse(char *str, int strlen)
# local vars:
#	-4(%ebp) - strlen/2
#
# registers:
#	%esi - string pointer
#	%ebx - strlen - 1
#	%edx - strlen - 1 - index
#	%edi - string index

	pushl %ebp		# save caller's base pointer
	movl %esp, %ebp		# create our stack frame

	subl $4, %esp		# save space for local var

	# load registers
	movl 8(%ebp), %esi
	movl 12(%ebp), %ebx
	movl %ebx, -4(%ebp)
	decl %ebx		# strlen - 1
	shrl $1, -4(%ebp)	# strlen/2

	movl $0, %edi
	movl %ebx, %edx

reverse_loop:
# registers:
#	%al - hold swapping char
#	%cl - hold swapping char

	cmpl -4(%ebp), %edi
	jge func_ret

	# swap the characters
	movb (%esi,%edi,1), %al
	movb (%esi,%edx,1), %cl
	movb %al, (%esi,%edx,1)
	movb %cl, (%esi,%edi,1)

	incl %edi
	decl %edx

	jmp reverse_loop

func_ret:
	movl %ebp, %esp		# restore stack and
	popl %ebp		# caller's base pointer

	ret			# %eax already holds retval
