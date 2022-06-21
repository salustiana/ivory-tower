#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#define CTRL_KEY(k)	((k) & 0x1f)
#define CLR_SCRN()	write(STDOUT_FILENO, "\x1b[2J\x1b[H", 7)
#define RENDER()	write(STDOUT_FILENO, scr.dsp, scr.dsp_siz)
#define DISP(x, y)	scr.dsp[scr.ws.ws_col*(y) + (x)]

#define MVRT()		DISP(scr.cpos.x++, scr.cpos.y) = ' '; \
			DISP(scr.cpos.x, scr.cpos.y) = '.';
#define MVLFT()		DISP(scr.cpos.x--, scr.cpos.y) = ' '; \
			DISP(scr.cpos.x, scr.cpos.y) = '.';
#define MVUP()		DISP(scr.cpos.x, scr.cpos.y--) = ' '; \
			DISP(scr.cpos.x, scr.cpos.y) = '.';
#define MVDN()		DISP(scr.cpos.x, scr.cpos.y++) = ' '; \
			DISP(scr.cpos.x, scr.cpos.y) = '.';

void err_exit(char *s)
{
	CLR_SCRN();
	perror(s);
	exit(1);
}

struct screen {
	struct winsize ws;
	char *dsp;
	int dsp_siz;
	struct {
		int x;
		int y;
	} cpos;
} scr;

void init_screen()
{
	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &scr.ws) == -1
	    || scr.ws.ws_col == 0)
		err_exit("init_screen");
	scr.dsp_siz = scr.ws.ws_col * scr.ws.ws_row;
	scr.dsp = malloc(scr.dsp_siz);
	for (int i=0; i < scr.dsp_siz; i++)
		scr.dsp[i] = ' ';
	scr.cpos.x = scr.ws.ws_col/2;
	scr.cpos.y = scr.ws.ws_row/2;
	DISP(scr.cpos.x, scr.cpos.y) = '.';
}

struct termios otty;
void disable_raw()
{
        if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &otty) == -1)
		err_exit("tcsetattr");
}

void enable_raw()
{
        if (tcgetattr(STDIN_FILENO, &otty) == -1)
		err_exit("tcgetattr");

        atexit(disable_raw);
        struct termios tty = otty;

	tty.c_iflag &= ~(ICRNL | IXON);
	tty.c_oflag &= ~(OPOST);
        tty.c_lflag &= ~(ECHO | ICANON | ISIG);
	tty.c_cc[VMIN] = 0;
	tty.c_cc[VTIME] = 1;

        if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &tty) == -1)
		err_exit("tcsetattr");
}

char read_key()
{
	int nread;
	char c;
	while ((nread = read(STDIN_FILENO, &c, 1)) != 1)
		if (nread == -1) {
			perror("read_key");
			exit(1);
		}
	return c;
}

void process_key()
{
	char c = read_key();
	switch (c) {
	case CTRL_KEY('c'):
		CLR_SCRN();
		write(STDOUT_FILENO, "BYE\r\n", 5);
		exit(0);
	case 'w':
		MVUP();
		RENDER();
		break;
	case 'a':
		MVLFT();
		RENDER();
		break;
	case 's':
		MVDN();
		RENDER();
		break;
	case 'd':
		MVRT();
		RENDER();
		break;
	default:
		printf("%c\r\n", c);
		break;
	}
}

int main()
{
	enable_raw();
	init_screen();
	RENDER();
	while (1)
		process_key();
	return 0;
}
