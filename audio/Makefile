CC = gcc
OBJS = main.c
CFLAGS = -Wall -Wextra -pedantic -std=c99 -g
INCLUDES = -iquote ./includes
LIBS = -lasound

a.out: ${OBJS}
	${CC} ${OBJS} ${INCLUDES} ${CFLAGS} ${LIBS}
