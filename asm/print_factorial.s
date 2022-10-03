	.section .bss
	.lcomm out_buff, 512
	.lcomm out_string, 512

	.section .text
	.globl _start
_start:

	pushl $12
	call fact
	addl $4, %esp

	pushl %eax
	pushl $out_buff
	call itoa
	addl $8, %esp

	# copy out_buff to out_string
	# until null termination
	movl $0, %edi
copy_buff_loop:
# registers:
#	%edi - buffer index
#	%cl - current byte
	
	movb out_buff(,%edi,1), %cl
	cmpb $0, %cl
	je end_copy_loop

	movb %cl, out_string(,%edi,1)
	incl %edi
	jmp copy_buff_loop

end_copy_loop:

	# add newline to print
	movb $'\n', out_string(,%edi,1)

	# print output
	movl $4, %eax
	movl $1, %ebx
	movl $out_string, %ecx
	movl $18, %edx
	int $0x80

	# exit
	movl $1, %eax
	movl $0, %ebx
	int $0x80
