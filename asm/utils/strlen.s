# function strlen to count a null
# terminated string's characters


	.section .text

	.globl strlen
	.type strlen, @function
strlen:		# int strlen(const char *str);
# registers:
#	%ecx - character count
#	%al - current char
#	%edx - current char address

	pushl %ebp
	movl %esp, %ebp

	movl $0, %ecx
	movl 8(%ebp), %edx

count_loop_begin:
	movb (%edx), %al	# get current char
	cmpb $0, %al
	je count_loop_end

	incl %ecx
	incl %edx
	jmp count_loop_begin
count_loop_end:
	movl %ecx, %eax
	popl %ebp
	ret
