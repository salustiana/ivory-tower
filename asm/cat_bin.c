#include <stdio.h>
#include <ctype.h>

int main()
{
	int c;
	int outword = 1;
	while ((c = getchar()) != EOF) {
		if (isprint(c)) {
			if (outword) {
				outword = 0;
				putchar(' ');
			}
			putchar(c);
		}
		else {
			outword = 1;
		}
	}
	putchar('\n');
}
