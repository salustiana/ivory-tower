	.include "record_def.s"
	.include "../utils/linux.s"

	.section .data
record1:
	# firstname
	.ascii "Baby\0"
	.rept 35		# repeat allocating 0 bytes
	.byte 0			# 35 times, to pad the
	.endr			# firstname to 40 bytes.

	# lastname
	.ascii "Paddintgon\0"
	.rept 29
	.byte 0
	.endr

	# address
	.ascii "4325 S Prairie, Tulsa, OK 55555\0"
	.rept 209		# padding to 240 bytes
	.byte 0
	.endr

	# age
	.long 55

record2:
	# firstname
	.ascii "Ricardo\0"
	.rept 32		# repeat allocating 0 bytes
	.byte 0			# 35 times, to pad the
	.endr			# firstname to 40 bytes.

	# lastname
	.ascii "Lopez\0"
	.rept 24
	.byte 0
	.endr

	# address
	.ascii "Av. Rivadavia 325\0"
	.rept 222		# padding to 240 bytes
	.byte 0
	.endr

	# age
	.long 35

file_name:
	.ascii "test.dat\0"

	.equ ST_FD,		-4


	.section .text
	.globl _start
_start:
	movl %esp, %ebp
	subl $4, %esp

	# open the file
	movl $SYS_OPEN, %eax
	movl $file_name, %ebx
	movl $0101, %ecx	# CREAT
	movl $0666, %edx
	int $LINUX_SYSCALL

	movl %eax, ST_FD(%ebp)	# store the fd

	# write first record
	pushl ST_FD(%ebp)
	pushl $record1
	call write_record
	addl $4, %esp

	# write second record
	pushl $record2
	call write_record
	addl $8, %esp

	# close file
	movl $SYS_CLOSE, %eax
	movl ST_FD(%ebp), %ebx
	int $LINUX_SYSCALL

	# exit
	movl $SYS_EXIT, %eax
	movl $0, %ebx
	int $LINUX_SYSCALL
