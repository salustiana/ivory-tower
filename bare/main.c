#include "utils.h"
#include "shaders.h"

#include <math.h>

#define SCR_WIDTH 1024
#define SCR_HEIGHT 768

void process_input(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, 1);
}

int main()
{
	GLFWwindow *window = get_window(SCR_WIDTH, SCR_HEIGHT);

	struct gl_color glc = hex_to_gl_color(0x003566);

	float vertices[] = {
		-0.5,	-0.5,	0,
		-0.5,	 0.5,	0,
		 0.5,	 0.5,	0,
	};

	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	unsigned int sp = build_shader("./move.vs", "./move.fs");

	int lateral_disp_uni = glGetUniformLocation(sp, "lateral_disp");
	int color_uni = glGetUniformLocation(sp, "color");

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		process_input(window);

		glClearColor(glc.R, glc.G, glc.B, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		float time = glfwGetTime();
		float lateral_disp = (sin(time * 2.0f) / 4.0f);
		float green = (sin(time * 2.0f) / 4.0f + 0.5f);

		glUseProgram(sp);
		glUniform1f(lateral_disp_uni, lateral_disp);
		glUniform4f(color_uni, 0.0f, green, 0.0f, 1.0f);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}
