#include "utils.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
	(void) window;
}  

GLFWwindow *get_window(int width, int height)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

	GLFWwindow *window = glfwCreateWindow(width, height,
			"Bare", NULL, NULL);

	if (!window) {
		perror("error in window creation");
		glfwTerminate();
		exit(1);
	}

	// set window-resize callback
	void framebuffer_size_callback(GLFWwindow *, int, int);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwMakeContextCurrent(window);

	// acquire OS specific functions
	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
		perror("error loading GLAD");
		exit(1);
	}

	glViewport(0, 0, width, height);

	return window;
}

struct gl_color hex_to_gl_color(unsigned long hex)
{
	struct gl_color glc;
	glc.R = ((hex & 0xFF0000) >> 16) / 255.0f;
	glc.G = ((hex & 0x00FF00) >>  8) / 255.0f;
	glc.B =  (hex & 0x0000FF) 	 / 255.0f;

	return glc;
}

unsigned int triangle_2d_vao(float x1, float y1, float x2, float y2, float x3, float y3)
{
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	float vertices[] = {
		x1, y1, 0,
		x2, y2, 0,
		x3, y3, 0,
	};

	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	return vao;
}

unsigned int rect_2d_vao(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
{
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	float vertices[] = {
		x1, y1, 0,
		x2, y2, 0,
		x3, y3, 0,
		x4, y4, 0,
	};

	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	unsigned int indeces[] = {
		0, 1, 2,
		0, 2, 3,
	};
	unsigned int ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indeces), indeces, GL_STATIC_DRAW);

	glBindVertexArray(0);
	return vao;
}
