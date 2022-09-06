#include "utils.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vtx_src = "#version 330 core\n"
	"layout (location = 0) in vec3 a_pos;\n"
	"void main()\n"
	"{\n"
	"	gl_Position = vec4(a_pos.x, a_pos.y, a_pos.z, 1.0);\n"
	"}\0";
const char *frag_src = "#version 330 core\n"
	"out vec4 frag_color;\n"
	"void main()\n"
	"{\n"
	"	frag_color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\0";

void process_input(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, 1);
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
}

int main()
{
	GLFWwindow *window = create_window(SCR_WIDTH, SCR_HEIGHT);

	// acquire OS specific functions
	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
		perror("error loading GLAD");
		return -1;
	}

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	struct GLColor glc = hex_to_glcolor(0x003566);

	float vertices[] = {
		-0.5,	-0.5,	0,
		-0.5,	 0.5,	0,
		 0.5,	 0.5,	0,
		 0.5,	-0.5,	0
	};
	unsigned int indeces[] = {
		0, 1, 2,
		0, 2, 3
	};
	unsigned int vao = create_index_vao(sizeof(vertices), vertices,
			sizeof(indeces), indeces);

	unsigned int sp = create_program(vtx_src, frag_src);
	glUseProgram(sp);

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
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}
