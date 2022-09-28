# PURPOSE:	print the maximum number on a
#		set of data items
#

# VARIABLES:
#	REGISTERS
# %edi - the index of the current data item
# %ebx - largest data item found
# %eax - current data item
#
#	MEMORY LOCATIONS
# data_items - contains the item data, which
#		must be null terminated
#

	.section .data

data_items:
	.long 3,67,234,52,55,123,3,21,0

	.section .text

	.globl _start

_start:
	movl $0, %edi			# initialize the index
	movl data_items(,%edi,4), %eax	# load first byte of data
	movl %eax,%ebx			# first number is the largest so far

loop_start:
	cmpl $0, %eax	# check if the data has ended
	je loop_exit

	incl %edi	# increment index and load next
	movl data_items(,%edi,4), %eax

	cmpl %ebx, %eax	# compare against biggest so far
	jle loop_start	# if eax is smaller, skip next line

	movl %eax, %ebx	# move this value as largest

	jmp loop_start	# loop

loop_exit:
exit:
	# exit syscall
	movl $1, %eax
			# ebx already holds the largest number
	int $0x80
