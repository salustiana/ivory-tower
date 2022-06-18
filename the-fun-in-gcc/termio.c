#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

int main()
{
        void enable_raw();

        enable_raw();

        int c;
        while (read(STDIN_FILENO, &c, 1) == 1) {
                if (iscntrl(c))
                        printf("%3d [ESC SEQ]\n", c);
                else
                        printf("%3d [%c]\n", c, c);
                //write(STDOUT_FILENO, &c, 1);
        }
        return 0;
}

static struct termios otty;

void disable_raw()
{
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &otty);
}

void enable_raw()
{
        tcgetattr(STDIN_FILENO, &otty);
        atexit(disable_raw);
        struct termios tty = otty;

        tty.c_lflag &= ~(ECHO | ICANON);

        tcsetattr(STDIN_FILENO, TCSAFLUSH, &tty);
}
