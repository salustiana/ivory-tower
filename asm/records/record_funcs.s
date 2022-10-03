	.include "record_def.s"
	.include "../utils/linux.s"

	.equ ST_READ_BUFF,	8
	.equ ST_WRITE_BUFF,	8
	.equ ST_FD,		12

	.section .text
	.globl read_record
	.globl write_record

	.type read_record, @function
read_record:
	pushl %ebp
	movl %esp, %ebp

	pushl %ebx		# XXX

	movl $SYS_READ, %eax
	movl ST_FD(%ebp), %ebx
	movl ST_READ_BUFF(%ebp), %ecx
	movl $REC_SIZE, %edx
	int $LINUX_SYSCALL

	popl %ebx		# XXX

	# we return the retval of the read syscall
	# %eax already holds it
	movl %ebp, %esp
	popl %ebp
	ret

	.type write_record, @function
write_record:
	pushl %ebp
	movl %esp, %ebp

	pushl %ebx		# XXX
	movl $SYS_WRITE, %eax
	movl ST_FD(%ebp), %ebx
	movl ST_WRITE_BUFF(%ebp), %ecx
	movl $REC_SIZE, %edx
	int $LINUX_SYSCALL

	# we return the retval of the write syscall
	# %eax already holds it
	movl %ebp, %esp
	popl %ebp
	ret
