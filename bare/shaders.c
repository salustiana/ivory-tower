#include "./shaders.h"

#include <stdlib.h>

unsigned int build_shader(const char *vtx_src_path, const char *frag_src_path)
{
	unsigned int sp;
	const char *vtx_src = read_file(vtx_src_path);
	if (vtx_src == NULL) {
		perror("error reading vertex source file");
		return 0;
	}
	const char *frag_src = read_file(frag_src_path);
	if (frag_src == NULL) {
		perror("error reading fragment source file");
		return 0;
	}

	// compile vertex shader
	unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vtx_src, NULL);
	glCompileShader(vs);
	// check for errors
	int success;
	char info_log[512];
	glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
	if (success != 1) {
		glGetShaderInfoLog(vs, 512, NULL, info_log);
		printf("vertex shader compile status: %d\ninfo log: %s\n",
				success, info_log);
		return 0;
	}

	// compile fragment shader
	unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &frag_src, NULL);
	glCompileShader(fs);
	// check for errors
	glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
	if (success != 1) {
		glGetShaderInfoLog(fs, 512, NULL, info_log);
		printf("fragment shader compile status: %d\ninfo log: %s\n",
				success, info_log);
		return 0;
	}

	// create shader program
	sp = glCreateProgram();
	glAttachShader(sp, vs);
	glAttachShader(sp, fs);
	glLinkProgram(sp);
	// check for errors
	glGetProgramiv(sp, GL_LINK_STATUS, &success);
	if (success != 1) {
		glGetProgramInfoLog(sp, 512, NULL, info_log);
		printf("program shader link status: %d\ninfo log: %s\n",
				success, info_log);
		return 0;
	}

	// delete shaders
	glDeleteShader(vs);
	glDeleteShader(fs);
	free((void *) vtx_src);
	free((void *) frag_src);

	return sp;
}
