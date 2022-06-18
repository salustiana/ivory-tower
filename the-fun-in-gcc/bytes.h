#include <stdio.h>

void print_bytes(void *obj, size_t size)
{
	char *b = obj;
	unsigned char m;
	for (size_t i = 0; i < size; i++) {
		m = 128;
		do {
			putchar(*b&m ? '1' : '0');
		} while (m >>= 1);
		putchar(' ');
		b++;
	}
	putchar('\n');
}
