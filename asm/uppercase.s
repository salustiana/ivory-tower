# output contents of input file
# with all lowercase characters
# converted to uppercase

	.equ SYS_EXIT,		1
	.equ SYS_READ,		3
	.equ SYS_WRITE,		4
	.equ SYS_OPEN,		5
	.equ SYS_CLOSE,		6
	.equ LINUX_SYSCALL,	0x80

	.equ STDIN,		0
	.equ STDOUT,		1
	.equ STDERR,		2

	.equ O_RDONLY,		0


	.section .data
error_msg:
	.ascii "syscall returned error status: "
error_code:
	.ascii "e\n"
	.equ ERROR_MSG_CHARS,	33


	.section .bss

	.equ BUFFSIZ,		512

	.lcomm BUFF, BUFFSIZ
	.lcomm FD_IN, 4
	.lcomm BYTES_READ, 4


	.section .text

	.equ ST_ARGC,		0
	.equ ST_ARGV_0,		4
	.equ ST_ARGV_1,		8

	.globl _start
_start:
	### initialize program ###
	# save stack pointer
	movl %esp, %ebp

	cmpl $2, ST_ARGC(%ebp)	# check if input file
	jge open_fd_in		# was provided

	movl $STDIN, FD_IN	# use stdin if not
	jmp loop

open_fd_in:
	# open syscall
	movl $SYS_OPEN, %eax
	movl ST_ARGV_1(%ebp), %ebx	# save filename
					# into %ebx
	movl $O_RDONLY, %ecx
	movl $0666, %edx		# permissions
	int $LINUX_SYSCALL		# call kernel
	cmpl $0, %eax
	jl panic

	movl %eax, FD_IN		# save fd
	jmp loop

loop:
	# read a block from input file
	movl $SYS_READ, %eax
	movl FD_IN, %ebx
	movl $BUFF, %ecx
	movl $BUFFSIZ, %edx
	int $LINUX_SYSCALL
	cmpl $0, %eax
	jl panic

	# save bytes read
	movl %eax, BYTES_READ

	# exit if we have reached EOF
	# or found an error
	cmpl $0, BYTES_READ
	jle loop_end

	# convert data to uppercase
	pushl BYTES_READ		# pass bytes read
	pushl $BUFF			# pass buff pointer
	call convert_to_upper
	addl $8, %esp			# restore %esp

	# write block to stdout
	movl $SYS_WRITE, %eax
	movl $STDOUT, %ebx
	movl $BUFF, %ecx
	movl BYTES_READ, %edx
	int $LINUX_SYSCALL
	cmpl $0, %eax
	jl panic

	# loop
	jmp loop

loop_end:
	# close the file
	movl $SYS_CLOSE, %eax
	movl FD_IN, %ebx
	int $LINUX_SYSCALL
	cmpl $0, %eax
	jl panic

	# exit
	movl $SYS_EXIT, %eax
	movl $0, %ebx
	int $LINUX_SYSCALL
	cmpl $0, %eax
	jl panic


# function
# convert_to_upper(char *str, size_t size)
# converts string to uppercase in place
#
# registers:
#	%eax - beggining of buffer
#	%ebx - length of buffer
#	%edi - current buffer offset
#	%cl - current byte from buffer
#	(first byte of %ecx)

.equ LOWCASE_A, 'a'
.equ LOWCASE_Z, 'z'
.equ UPPER_OFFSET, 'A' - 'a'

.equ ST_BUFF_LEN, 12
.equ ST_BUFF, 8

convert_to_upper:
	# stack frame
	pushl %ebp
	movl %esp, %ebp

	# setup registers
	movl ST_BUFF(%ebp), %eax
	movl ST_BUFF_LEN(%ebp), %ebx
	movl $0, %edi

	# if buff len is zero, leave
	cmpl $0, %ebx
	je convert_loop_end

convert_loop:
	movb (%eax,%edi,1), %cl	# load byte
	cmpb $LOWCASE_A, %cl	# skip if not in
	jl next_byte		# lowercase bounds
	cmpb $LOWCASE_Z, %cl
	jg next_byte

	addb $UPPER_OFFSET, (%eax,%edi,1)
	jmp next_byte

next_byte:
	incl %edi
	cmpl %ebx, %edi		# return if reached end
	jge convert_loop_end

	jmp convert_loop

convert_loop_end:
	movl %ebp, %esp
	popl %ebp
	ret

panic:
	movb %al, error_code
	movl $4, %eax
	movl $2, %ebx
	movl $error_msg, %ecx
	movl $ERROR_MSG_CHARS, %edx
	int $LINUX_SYSCALL

	movl $1, %eax
	movl $1, %ebx
	int $LINUX_SYSCALL
