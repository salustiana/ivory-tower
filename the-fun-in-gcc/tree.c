#include <unistd.h>
#include <libgen.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <regex.h>

#define CLR_YELLOW  "\x1b[33m"
#define CLR_GRAY   "\x1b[90m"
#define CLR_CYAN    "\x1b[36m"
#define CLR_RESET   "\x1b[0m"

int all = 0;
int size = 0;
int ign = 0;
regex_t ignore;

// apply fnc to all files in dir
void dirwalk(char *dir, void (*fnc)(char *))
{
	DIR *dp;
	if ((dp = opendir(dir)) == NULL) {
		perror("tree");
		return;
	}
	struct dirent *dntp;
	char path[PATH_MAX];
	while ((dntp = readdir(dp)) != NULL) {
		if ((strcmp(dntp->d_name, ".") == 0)
		 || (strcmp(dntp->d_name, "..") == 0))
			continue;
		if (snprintf(path, PATH_MAX, "%s/%s", dir, dntp->d_name)
		 >= PATH_MAX) {
			fprintf(stderr, "tree: path too long: %s\n", path);
			continue;
		}
		fnc(path);
	}
	closedir(dp);
}

void print_file(char *s, int indent, long unsigned siz, int isdir)
{
	int first = indent == 0 ? 1 : 0;
	printf(CLR_GRAY);
	while (--indent > 0)
		printf(" |  ");
	if (!first)
		printf(" \\_ ");
	printf(isdir ? CLR_CYAN"%s/" : CLR_RESET"%s", s);
	if (siz && !isdir)
		printf(CLR_YELLOW"  %lu", siz);
	printf(CLR_RESET"\n");
}

void tree(char *path)
{
	static int indent = 0;

	if (ign && regexec(&ignore, path, 0, NULL, 0) == 0)
		return;

	struct stat stbuf;
	if (stat(path, &stbuf) != 0) {
		perror("tree");
		return;
	}

	char *bsname = basename(path);
	if (!all && *bsname == '.')
		return;

	long unsigned siz = size ? stbuf.st_size : 0;

	if ((stbuf.st_mode & S_IFMT) == S_IFDIR) {
		print_file(bsname, indent, siz, 1);
		indent++;
		dirwalk(path, tree);
		indent--;
		return;
	}
	print_file(bsname, indent, siz, 0);
}

int main(int argc, char *argv[])
{
	int opt;
	while((opt = getopt(argc, argv, "asi:")) != -1)
		switch (opt) {
		case 'a':
			all = 1;
			break;
		case 's':
			size = 1;
			break;
		case 'i':
			if (regcomp(&ignore, optarg, 0) != 0) {
				perror("tree");
				return 1;
			}
			ign = 1;
			break;
		default:
			fprintf(stderr, "Usage: %s [-a] [-s] [path]...",
				argv[0]);
		}

	argc -= optind - 1;
	argv += optind - 1;

	if (argc == 1)
		tree(realpath(".", NULL));
	else {
		char rpath[PATH_MAX];
		while (--argc > 0) {
			realpath(*++argv, rpath);
			tree(rpath);
		}
	}
	return 0;
}
