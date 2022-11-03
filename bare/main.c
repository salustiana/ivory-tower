#include <math.h>
#include <malloc.h>

#include "utils.h"
#include "shader.h"
#include "camera.h"

#include "cglm/cglm.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define SCR_WIDTH	800
#define SCR_HEIGHT	600

#define LEN(a)		sizeof((a)) / sizeof((a)[0])
#define VEC3(x, y, z)	(float []) {(x), (y), (z)}

inline void print_mat(float m[][4])
{
	for(int i=0; i<4; i++) {
		for (int j=0; j<4; j++)
			printf("%f\t", m[i][j]);
		putchar('\n');
	}
	putchar('\n');
}

double delta = 0;

vec4 *model;
vec4 *view;
vec4 *projection;
int model_u;
int view_u;
int projection_u;

struct camera cam;
float cam_speed = 3;

void process_input(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, 1);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		translate_cam(&cam, cam.dir, cam_speed * delta);
		set_view(&cam, view);
		glUniformMatrix4fv(view_u, 1, GL_FALSE, (float *) view);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		translate_cam(&cam, cam.dir, -cam_speed * delta);
		set_view(&cam, view);
		glUniformMatrix4fv(view_u, 1, GL_FALSE, (float *) view);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		translate_cam(&cam, cam.left, cam_speed * delta);
		set_view(&cam, view);
		glUniformMatrix4fv(view_u, 1, GL_FALSE, (float *) view);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		translate_cam(&cam, cam.left, -cam_speed * delta);
		set_view(&cam, view);
		glUniformMatrix4fv(view_u, 1, GL_FALSE, (float *) view);
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		spin_cam(&cam, cam.left, cam_speed * delta/GLM_PI);
		set_view(&cam, view);
		glUniformMatrix4fv(view_u, 1, GL_FALSE, (float *) view);
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		spin_cam(&cam, cam.left, -cam_speed * delta/GLM_PI);
		set_view(&cam, view);
		glUniformMatrix4fv(view_u, 1, GL_FALSE, (float *) view);
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		spin_cam(&cam, cam.up, -cam_speed * delta/GLM_PI);
		set_view(&cam, view);
		glUniformMatrix4fv(view_u, 1, GL_FALSE, (float *) view);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		spin_cam(&cam, cam.up, cam_speed * delta/GLM_PI);
		set_view(&cam, view);
		glUniformMatrix4fv(view_u, 1, GL_FALSE, (float *) view);
	}
}

double prev_mouse_x = (float) SCR_WIDTH/2, prev_mouse_y = (float) SCR_HEIGHT/2;
double pitch = 0, yaw = 0;
const float sensitivity = 0.1f;

void mouse_callback(GLFWwindow* window, double mouse_x, double mouse_y)
{
	double delta_x = mouse_x - prev_mouse_x;
	double delta_y = mouse_y - prev_mouse_y;
	prev_mouse_x = mouse_x;
	prev_mouse_y = mouse_y;

	pitch += -delta_y * sensitivity * delta; // y-coordinates are reversed
	yaw += delta_x * sensitivity * delta;

	if (pitch >= GLM_PI_2)
		pitch = GLM_PI_2 - 0.01;
	if (pitch <= -GLM_PI_2)
		pitch = -GLM_PI_2 + 0.01;

	set_cam_dir(&cam, VEC3(cos(pitch)*sin(yaw), sin(pitch), -cos(pitch)*cos(yaw)));
	set_view(&cam, view);
	glUniformMatrix4fv(view_u, 1, GL_FALSE, (float *) view);
}

double fov = GLM_PI_4;
void scroll_callback(GLFWwindow* window, double x_offset, double y_offset)
{
	fov -= y_offset * sensitivity * delta * 6;
	if (fov < GLM_PI / 8)
		fov = GLM_PI / 8;
	if (fov > GLM_PI_2)
		fov = GLM_PI_2; 

	glm_perspective(fov, ((float) SCR_WIDTH) / ((float) SCR_HEIGHT), 0.1f, 100.0f, projection);
	glUniformMatrix4fv(projection_u, 1, GL_FALSE, (float *) projection);
}

int main()
{
	GLFWwindow *window = get_window(SCR_WIDTH, SCR_HEIGHT);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetCursorPosCallback(window, mouse_callback);  
	glfwSetScrollCallback(window, scroll_callback); 

	struct gl_color glc = hex_to_gl_color(0x003566);

	unsigned int sp = build_shader("./shaders/move.vs", "./shaders/move.fs");

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

	t_data = stbi_load("./textures/Three-Scenes-800x600.jpg", &t_width, &t_height, &t_nr_channels, 0);
	if (t_data == NULL)
		perror("error loading texture");
	unsigned int texture2;
	glGenTextures(1, &texture2);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, t_width, t_height, 0, GL_RGB, GL_UNSIGNED_BYTE, t_data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(t_data);
	glUseProgram(sp);
	glUniform1i(glGetUniformLocation(sp, "texture_uni2"), 1);

	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	float vertices[] = {
	    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	};

	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void *) (3*sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	model = memalign(16, 16 * sizeof(float));
	view = memalign(16, 16 * sizeof(float));
	projection = memalign(16, 16 * sizeof(float));

	model_u = glGetUniformLocation(sp, "model");
	view_u = glGetUniformLocation(sp, "view");
	projection_u = glGetUniformLocation(sp, "projection");

	glEnable(GL_DEPTH_TEST);

	float cube_positions[][3] = {
		{0.0f,   0.0f,  0.0f },
		{2.0f,   5.0f, -15.0f},
		{-1.5f, -2.2f, -2.5f },
		{-3.8f, -2.0f, -12.3f},
		{2.4f,  -0.4f, -3.5f },
		{-1.7f,  3.0f, -7.5f },
		{1.3f,  -2.0f, -2.5f },
		{1.5f, 	 2.0f, -2.5f },
		{1.5f,   0.2f, -1.5f },
		{-1.3f,  1.0f, -1.5f },
	};

	set_cam(&cam, VEC3(0, 0, 3), VEC3(0, 0, -1), VEC3(0, 1, 0));

	glm_lookat(cam.pos, cam.aim, cam.up, view);
	glUniformMatrix4fv(view_u, 1, GL_FALSE, (float *) view);

	glm_mat4_identity(projection);
	glm_perspective(fov, ((float) SCR_WIDTH) / ((float) SCR_HEIGHT), 0.1f, 100.0f, projection);
	glUniformMatrix4fv(projection_u, 1, GL_FALSE, (float *) projection);

	//while (!glfwWindowShouldClose(window)) {
	for (int frames = 0; !glfwWindowShouldClose(window); frames++) {
		double frame_start = glfwGetTime();

		glfwPollEvents();
		process_input(window);

		glClearColor(glc.R, glc.G, glc.B, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(vao);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		for (int i = 0; i < LEN(cube_positions); i++) {
			glm_mat4_identity(model);
			glm_translate(model, cube_positions[i]);
			glm_rotate(model,((float)LEN(cube_positions)-i)*glfwGetTime()*glm_rad(10),VEC3(1,1.3,0));
			glUniformMatrix4fv(model_u, 1, GL_FALSE, (float *) model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glfwSwapBuffers(window);

		delta = glfwGetTime() - frame_start;
		if (frames % 200 == 0)
			printf("fps: %lf\n", 1/delta);
	}

	glfwTerminate();
	return 0;
}
