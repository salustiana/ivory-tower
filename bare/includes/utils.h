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
unsigned int triangle_2d_vao(float x1, float y1, float x2, float y2, float x3, float y3);
unsigned int rect_2d_vao(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
