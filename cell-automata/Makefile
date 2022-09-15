CC = gcc
OBJS = main.o glad.o shader.o utils.o
CFLAGS = -Wall -Wextra -std=c99
INCLUDES = -I ./includes
LIBS = -lGL -lglfw -ldl -lm -lmi

a.out: ${OBJS} ./shaders
	${CC} ${OBJS} ${INCLUDES} ${CFLAGS} ${LIBS}

main.o: main.c
	${CC} -c main.c ${INCLUDES}

glad.o: glad.c
	${CC} -c glad.c ${INCLUDES}

shader.o: shader.c
	${CC} -c shader.c ${INCLUDES}

utils.o: utils.c
	${CC} -c utils.c ${INCLUDES}
