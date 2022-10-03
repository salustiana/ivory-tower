# print first and lastname of every record

	.include "../utils/linux.s"
	.include "record_def.s"

	.section .data
file_name:
	.ascii "test.dat\0"
newline_str:
	.ascii "\n\0"
space_str:
	.ascii " \0"


	.section .bss
	.lcomm record_buff, REC_SIZE


	.section .text
	.globl _start
_start:
	
	.equ ST_IN_FD,		-4

	movl %esp, %ebp
	subl $4, %esp

	# open the file
	movl $SYS_OPEN, %eax
	movl $file_name, %ebx
	movl $0, %ecx		# read only
	movl $0666, %edx
	int $LINUX_SYSCALL

	# save fd
	movl %eax, ST_IN_FD(%ebp)

	# these are the same for every read_record call
	pushl ST_IN_FD(%ebp)
	pushl $record_buff
read_record_loop:
	call read_record

	# if we read less characters than
	# solicited, we're at EOF, or an
	# error has occurred
	cmpl $REC_SIZE, %eax
	jne finished_reading

	# print firstname, lastname and newline
	pushl $record_buff + REC_FIRSTNAME
	call print
	pushl $space_str
	call print
	pushl $record_buff + REC_LASTNAME
	call print
	pushl $newline_str
	call print
	addl $16, %esp

	jmp read_record_loop

finished_reading:
	movl $SYS_EXIT, %eax
	movl $0, %ebx
	int $LINUX_SYSCALL
