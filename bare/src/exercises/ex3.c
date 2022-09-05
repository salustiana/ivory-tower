#include "utils.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vtx_src = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";
const char *frag_src_orange = "#version 330 core\n"
	"out vec4 frag_color;\n"
	"void main()\n"
	"{\n"
	"	frag_color = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
	"}\0";
const char *frag_src_yellow = "#version 330 core\n"
	"out vec4 frag_color;\n"
	"void main()\n"
	"{\n"
	"	frag_color = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
	"}\0";

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
		-0.5,	-0.5,	0,	// botleft
		-0.5,	 0.5,	0,	// topleft
		 0.5,	 0.5,	0,	// topright
	};
	float vertices2[] = {
		 0.5,	 0.5,	0,	// topright
		 0.5,	-0.5,	0,	// botright
		-0.5,	-0.5,	0	// botleft
	};
	unsigned int vao = create_vao(sizeof(vertices), vertices);
	unsigned int vao2 = create_vao(sizeof(vertices2), vertices2);

	unsigned int sp_orange = create_program(vtx_src, frag_src_orange);
	unsigned int sp_yellow = create_program(vtx_src, frag_src_yellow);

	// main loop
	while(!glfwWindowShouldClose(window))
	{
		glfwPollEvents();    

		glClearColor(glc.R, glc.G, glc.B, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// draw triangle
		glUseProgram(sp_orange);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glUseProgram(sp_yellow);
		glBindVertexArray(vao2);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}