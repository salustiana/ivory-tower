#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

GLFWwindow *create_window(int width, int height);
unsigned int create_program(const char *vtx_src, const char *frag_src);
unsigned int create_vao(size_t vsize, float *vertices);
unsigned int create_index_vao(size_t vsize, float *vertices,
		size_t isize, unsigned int *indeces);
struct GLColor {
	GLfloat R;
	GLfloat G;
	GLfloat B;
};
struct GLColor hex_to_glcolor(unsigned long);
