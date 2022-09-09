#include <math.h>

#include "utils.h"
#include "shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define SCR_WIDTH	1024
#define SCR_HEIGHT	768

void process_input(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, 1);
}

int main()
{
	GLFWwindow *window = get_window(SCR_WIDTH, SCR_HEIGHT);

	struct gl_color glc = hex_to_gl_color(0x003566);

	unsigned int sp = build_shader("./shaders/move.vs", "./shaders/move.fs");

	int lateral_disp_uni = glGetUniformLocation(sp, "lateral_disp");
	int color_uni = glGetUniformLocation(sp, "color");

	stbi_set_flip_vertically_on_load(1);
	int t_width, t_height, t_nr_channels;
	unsigned char *t_data = stbi_load("./textures/container.jpg", &t_width, &t_height, &t_nr_channels, 0);
	if (t_data == NULL)
		perror("error loading texture");
	unsigned int texture;
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, t_width, t_height, 0, GL_RGB, GL_UNSIGNED_BYTE, t_data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(t_data);
	glUseProgram(sp);
	glUniform1i(glGetUniformLocation(sp, "texture_uni"), 0);

	t_data = stbi_load("./textures/awesomeface.png", &t_width, &t_height, &t_nr_channels, 0);
	if (t_data == NULL)
		perror("error loading texture");
	unsigned int texture2;
	glGenTextures(1, &texture2);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, t_width, t_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, t_data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(t_data);
	glUseProgram(sp);
	glUniform1i(glGetUniformLocation(sp, "texture_uni2"), 1);

	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	float vertices[] = {
		// positions          // colors           // texture coords
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,   // top left 
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
	};

	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void *) (3*sizeof(float)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void *) (6*sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	unsigned int indeces[] = {
		0, 1, 2,
		0, 2, 3,
	};
	unsigned int ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indeces), indeces, GL_STATIC_DRAW);

	glBindVertexArray(0);

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

		glBindVertexArray(vao);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}
