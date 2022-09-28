# return first instruction
#
	.section .data
carac:
	.byte 51, 10

	.section .text
	.globl _start

_start:

	mov $4, %eax
	mov $1, %ebx
	mov $carac, %ecx
	mov $2, %edx
	int $0x80
	jmp _start

exit:
	# exit syscall
	mov $1, %eax
	mov $0, %ebx
	int $0x80
