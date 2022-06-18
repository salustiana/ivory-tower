#include <stdio.h>

#include "mem.h"

char *sbrk(int);

int main()
{
	printf("\n------------------------------------------\n");
	printf( "start of program\n");
	printf( "\n> memstate();\n");
	memstate();

	printf("------------------------------------------\n");
	printf( "when a new block is needed, allocm\n"
		"acquires at least %d bytes for the\n"
		"new block, since syscalls are expensive\n",
		NALLOC*sizeof(Header));


	printf( "> allocm(32);\n"
		"> memstate();\n");
	allocm(32);
	memstate();
	printf("------------------------------------------\n");

	printf( "allocm returns a multiple of %d bytes, to satisfy\n"
		"all alignment requirements simultaneously.\n"
		"it also uses up an extra %d bytes to keep\n"
		"track of the used space\n",
		sizeof(Header), sizeof(Header));

	printf( "\n> allocm(12);  recieves 16 + 16 = 32 bytes\n"
		"> memstate();\n");
	allocm(12);
	memstate();

	printf( "\n> allocm(3020); recieves 3024 + 16 = 3040 bytes\n"
		"> memstate();\n");
	allocm(3020);
	memstate();

	printf( "\n> allocm(16000); must ask for a new block\n"
		"> memstate();\n");
	allocm(16000);
	memstate();
	printf("------------------------------------------\n");

	printf( "allocm starts searching for an appropriate block\n"
		"after the block marked with the asterisk\n"
		"so if we now ask for 40 bytes, they will be taken\n"
		"from the last block\n");

	printf( "\n> void *x = allocm(40);\n"
		"> memstate();\n");
	void *x = allocm(40);
	memstate();
	printf("------------------------------------------\n");

	printf( "freeing solicited memory with freem causes\n"
		"that memory to be restored to an appropriate\n"
		"block, regarding memory address order.\n"
		"if freed memory reaches adjacent blocks, they\n"
		"will be merged together\n");

	printf( "\n> freem(x);\n"
		"> memstate();\n");
	freem(x);
	memstate();
	printf("------------------------------------------\n");

	printf( "if a block's memory is depleted entirely,\n"
		"it will be unlinked from the chain\n");

	printf( "\n> x = allocm(13240);\n"
		"> memstate();\n");
	x = allocm(13240);
	memstate();
	printf("------------------------------------------\n");

	printf( "freeing that acquired memory will restore\n"
		"the block\n");

	printf( "\n> freem(x);\n"
		"> memstate();\n");
	freem(x);
	memstate();
	printf("------------------------------------------\n");
}
