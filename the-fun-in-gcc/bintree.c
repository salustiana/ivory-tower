#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define nelem(a) (sizeof(a) / sizeof(a[0]))

int pow2(int exp)
{
	unsigned int res = 1;
	res <<= exp;
	return res;
}

struct node {
	char *s;
	struct node *left;
	struct node *right;
};

struct node *makenode(char *s, struct node *left, struct node *right)
{
	struct node *n = calloc(1, sizeof(struct node));
	n->s = s;
	n->left = left;
	n->right = right;
	return n;
}

struct node *find(struct node *n, char *s)
{
	int sc = strcmp(s, n->s);
	if (sc == 0)
		return n;
	if (sc < 0)
		return n->left ? find(n->left, s) : NULL;
	return n->right ? find(n->right, s) : NULL;
}

void insert(struct node *n, char *s)
{
	int sc = strcmp(s, n->s);
	if (sc == 0)
		return;
	if (sc < 0) {
		if (n->left)
			return insert(n->left, s);
		n->left = makenode(s, NULL, NULL);
		return;
	}
	if (n->right)
		return insert(n->right, s);
	n->right = makenode(s, NULL, NULL);
	return;
}

void repr_tree(char *lines[], struct node *n, int d, int depth, int slen)
{
	/*
	* slen = 5 (iiiii)
	* depth = 4
	* .....................iiiii..................... d=1
	* .........iiiii...................iiiii......... d=2
	* ...iiiii.......iiiii.......iiiii.......iiiii... d=3
	* iiiii.iiiii.iiiii.iiiii.iiiii.iiiii.iiiii.iiiii d=4
	*
	* line characters (lc):
	* slen*2^depth + 2^depth - 1 = (slen+1)*2^depth - 1
	*
	* displacement between words in a line must be:
	* 2^(depth-d)*(slen+1) - (slen-1)
	*
	* and the first padding is half of that minus one
	* (2^(depth-d)*(slen+1) - (slen-1))/2 - 1
	*
	*/
	if (d > depth)
		return;
	int l = d - 1;
	int c = strlen(lines[0]) - 1;
	while (lines[l][c] == ' ' && c > 0)
		c--;
	if (c == 0)
		c -= (pow2(depth - d)*(slen+1) - (slen-1))/2 - 1;
	c += pow2(depth-d)*(slen+1) - (slen-1);
	int i = 0;
	if (n)
		while (i < slen && n->s[i]) {
			lines[l][c+i] = n->s[i];
			i++;
		}
	while (i < slen) {
		lines[l][c+i] = '.';
		i++;
	}
	repr_tree(lines, n ? n->left : NULL, d + 1, depth, slen);
	repr_tree(lines, n ? n->right : NULL, d + 1, depth, slen);
}

void print_tree(struct node *n, int depth, int slen)
{
	int line_memb = slen*pow2(depth) + 1;
	char *lines[depth];
	for (int l=0; l<depth; l++) {
		lines[l] = calloc(line_memb, sizeof(char));
		for (int c=0; c < line_memb-1; c++)
			lines[l][c] = ' ';
		lines[l][line_memb-1] = '\0';
	}

	repr_tree(lines, n, 1, depth, slen);
	for (int l=0; l < depth; l++) {
		printf("%s", lines[l]);
		for (int j=depth-l; j >= 0; j--)
			putchar('\n');
	}
}

int main()
{
	int depth = 5;
	int slen = 5;
	struct node *n = makenode("jarro", NULL, NULL);
	char *insrt[] = {"foca", "roca", "antes", "mora", "cosa", "xilo", "rico"};
	for (int i=0; i < nelem(insrt); i++)
		insert(n, insrt[i]);
	print_tree(n, depth, slen);
}
