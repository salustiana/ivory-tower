#include <stdio.h>
#include <stdlib.h>

/*
 * read file to dynamically allocated buffer
 * and return a pointer to it, or NULL if
 * there was an error
 */
char *read_file(const char *file_path)
{
	char *contents = NULL;
	FILE *fp = fopen(file_path, "r");

	if (fp == NULL) {
		perror("could not open file\n");
		return NULL;
	}

	if (fseek(fp, 0L, SEEK_END) != 0) {
		perror("could not reach end of file\n");
		return NULL;
	}
	long file_bytes = ftell(fp);
	if (file_bytes == -1) {
		perror("error getting position in file\n");
		return NULL;
	}

	contents = malloc(sizeof(char) * (file_bytes + 1));

	if (fseek(fp, 0L, SEEK_SET) != 0) {
		perror("could not return to file start\n");
		free(contents);
		return NULL;
	}

	size_t bytes_read = fread(contents, sizeof(char), file_bytes, fp);
	if (bytes_read != (size_t) file_bytes) {
		fprintf(stderr, "bytes_read: %lu\tfile_bytes: %lu\n",
			bytes_read, file_bytes);
	}

	if (ferror(fp)) {
		perror("error reading file\n");
		free(contents);
		return NULL;
	}

	contents[bytes_read] = '\0';

	return contents;
}
