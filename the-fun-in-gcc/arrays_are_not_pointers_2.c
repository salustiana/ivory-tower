#include <stdio.h>

extern char *a;
extern char b[];

int main()
{
	printf("b[2]: %c\n", b[2]);
	printf("a[2]: %c\n", a[2]);
}
