#include "utils.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
	(void) window;
}  
GLFWwindow *create_window(int width, int height)
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

	return window;
}

unsigned int create_program(const char *vtx_src, const char *frag_src)
{
	// create vertex shader
	unsigned int vs;
	vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vtx_src, NULL);
	glCompileShader(vs);
	// check shader compilation
	int success;
	char info_log[512];
	glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
	if (success != 1) {
		glGetShaderInfoLog(vs, 512, NULL, info_log);
		printf("vertex shader compile status: %d\ninfo log: %s\n",
				success, info_log);
		exit(1);
	}

	// create fragment shader
	unsigned int fs;
	fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &frag_src, NULL);
	glCompileShader(fs);
	// check shader compilation
	glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
	if (success != 1) {
		glGetShaderInfoLog(fs, 512, NULL, info_log);
		printf("fragment shader compile status: %d\ninfo log: %s\n",
				success, info_log);
		exit(1);
	}

	// create shader program
	unsigned int sp;
	sp = glCreateProgram();
	glAttachShader(sp, vs);
	glAttachShader(sp, fs);
	glLinkProgram(sp);
	glGetProgramiv(sp, GL_LINK_STATUS, &success);
	if (success != 1) {
		glGetProgramInfoLog(sp, 512, NULL, info_log);
		printf("program shader link status: %d\ninfo log: %s\n",
				success, info_log);
		exit(1);
	}

	// delete shaders
	glDeleteShader(vs);
	glDeleteShader(fs);

	return sp;
}

unsigned int create_vao(size_t vsize, float *vertices)
{
	// create vertex array object
	unsigned int vao;
	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);

	// create a buffer object to hold vertices
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vsize, vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// TODO: unbind vao before returning
	return vao;
}

unsigned int create_index_vao(size_t vsize, float *vertices,
		size_t isize, unsigned int *indeces)
{
	// create vertex array object
	unsigned int vao;
	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);

	// create a buffer object to hold vertices
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vsize, vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// create an element buffer object for index-based drawing
	unsigned int ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, isize, indeces, GL_STATIC_DRAW);

	// TODO: unbind vao before returning
	return vao;
}

struct GLColor hex_to_glcolor(unsigned long hex)
{
	struct GLColor glc;
	glc.R = ((hex & 0xFF0000) >> 16) / 255.0f;
	glc.G = ((hex & 0x00FF00) >>  8) / 255.0f;
	glc.B =  (hex & 0x0000FF) 	 / 255.0f;

	return glc;
}
