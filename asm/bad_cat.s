# read file specified in argv[1]
# and print its contents to stdout

	.section .bss
	.lcomm mi_buff, 2048

	.section .text
	.globl _start

_start:
	# open file
	movl $5, %eax		# open syscall
	movl 8(%esp), %ebx	# filename from argv[1]
	movl $0, %ecx		# read mode
	movl $0666, %edx	# permissions
	int $0x80

	movl %eax, %ebx		# store fd in ebx
	# read to mi_buff
	movl $3, %eax
	movl $mi_buff, %ecx
	movl $2048, %edx
	int $0x80

	# save bytes read
	movl %eax, %edx

	# write mi_buff to stdout
	movl $4, %eax
	movl $1, %ebx
	movl $mi_buff, %ecx
	# %edx already holds bytes to write
	int $0x80

exit:
	movl $1, %eax
	int $0x80
