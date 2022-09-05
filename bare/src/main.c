#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *v_shader_source = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";
const char *f_shader_source = "#version 330 core\n"
	"out vec4 frag_color;\n"
	"void main()\n"
	"{\n"
	"	frag_color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\0";

void process_input(GLFWwindow *);
void framebuffer_size_callback(GLFWwindow *, int, int);
struct GLColor {
	GLfloat R;
	GLfloat G;
	GLfloat B;
};
struct GLColor hex_to_glcolor(unsigned long );

int main()
{
	GLFWwindow *create_window(int, int);

	GLFWwindow *window = create_window(SCR_WIDTH, SCR_HEIGHT);

	// acquire OS specific functions
	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
		perror("error loading GLAD");
		return -1;
	}

	glViewport(0, 0, 800, 600);

	struct GLColor glc = hex_to_glcolor(0x003566);

	// create a vertex array object
	unsigned int vao;
	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);

	// create a buffer object and set it to hold triangle vertices
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	float vertices[] = {
		-0.5,	-0.5,	0,
		   0,	 0.5,	0,
		 0.5,	-0.5,	0
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);


	// create vertex shader
	unsigned int vs;
	vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &v_shader_source, NULL);
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
	glShaderSource(fs, 1, &f_shader_source, NULL);
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

	// use program and delete shaders
	glUseProgram(sp);
	glDeleteShader(vs);
	glDeleteShader(fs);


	// main loop
	while(!glfwWindowShouldClose(window))
	{
		glfwPollEvents();    
		process_input(window);

		glClearColor(glc.R, glc.G, glc.B, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// draw triangle
		glUseProgram(sp);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
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

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
	(void) window;
}  

void process_input(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, 1);
}

struct GLColor hex_to_glcolor(unsigned long hex)
{
	struct GLColor glc;
	glc.R = ((hex & 0xFF0000) >> 16) / 255.0f;
	glc.G = ((hex & 0x00FF00) >>  8) / 255.0f;
	glc.B =  (hex & 0x0000FF) 	 / 255.0f;

	return glc;
}
