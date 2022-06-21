#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <stdio.h>

static struct termios otty;

void disable_raw()
{
        if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &otty) == -1) {
		perror("tcsetattr");
		exit(1);
	}
}

void enable_raw()
{
        if (tcgetattr(STDIN_FILENO, &otty) == -1) {
		perror("tcgetattr");
		exit(1);
	}

        atexit(disable_raw);
        struct termios tty = otty;

	tty.c_iflag &= ~(ICRNL | IXON);
	tty.c_oflag &= ~(OPOST);
        tty.c_lflag &= ~(ECHO | ICANON);

        if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &tty) == -1) {
		perror("tcsetattr");
		exit(1);
	}
}
