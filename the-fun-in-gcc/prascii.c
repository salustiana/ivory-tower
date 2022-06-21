#include <unistd.h>
#include <ctype.h>
#include <stdio.h>

#include "termio.h"

int main()
{
        void enable_raw();

        enable_raw();

	char c;
        while (read(STDIN_FILENO, &c, 1) == 1) {
                if (iscntrl(c))
                        printf("%3d [ESC SEQ]\r\n", c);
                else
                        printf("%3d [%c]\r\n", c, c);
                //write(STDOUT_FILENO, &c, 1);
        }
        return 0;
}
