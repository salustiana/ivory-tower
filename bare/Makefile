CC = gcc
OBJS = main.c glad.o read_file.c shader.c camera.c utils.c
CFLAGS = -Wall -Wextra -std=c99
INCLUDES = -I ./includes
LIBS = -lGL -lglfw -ldl -lm

a.out: ${OBJS} ./shaders
	${CC} ${OBJS} ${INCLUDES} ${CFLAGS} ${LIBS}

glad.o: glad.c
	${CC} -c glad.c ${INCLUDES}
