#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

GLFWwindow *get_window(int width, int height);

struct gl_color {
	GLfloat R;
	GLfloat G;
	GLfloat B;
};

struct gl_color hex_to_gl_color(unsigned long);

void print_cells(int m[10][10]);
