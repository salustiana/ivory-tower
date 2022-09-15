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

void print_cells(int m[10][10])
{
	for(int x=0; x<10; x++) {
		for (int y=0; y<10; y++)
			printf("%d,", m[x][y]);
		putchar('\n');
	}
	putchar('\n');
}


