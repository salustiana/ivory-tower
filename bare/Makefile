CC = gcc
OBJS = main.o glad.o shaders.o utils.o
CFLAGS = -Wall -Wextra -std=c99
INCLUDES = -I ./includes
LIBS = -lGL -lglfw -ldl -lm -lmi

prog: ${OBJS}
	${CC} -o prog ${OBJS} ${INCLUDES} ${CFLAGS} ${LIBS}

main.o: main.c
	${CC} -c main.c ${INCLUDES}

glad.o: glad.c
	${CC} -c glad.c ${INCLUDES}

shaders.o: shaders.c
	${CC} -c shaders.c ${INCLUDES}

utils.o: utils.c
	${CC} -c utils.c ${INCLUDES}
