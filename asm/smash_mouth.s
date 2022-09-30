	.section .data
smash:
	.ascii "you're a rockstar!"
filenam:
	.ascii "./heynow.txt"

	.section .text
	.globl _start
_start:
	# open
	movl $5, %eax
	movl $filenam, %ebx
	movl $03101, %ecx
	movl $0666, %edx
	int $0x80

	# write
	movl %eax, %ebx
	movl $4, %eax
	movl $smash, %ecx
	movl $18, %edx
	int $0x80

	# exit
	movl $1, %eax
	movl $0, %ebx
	int $0x80
