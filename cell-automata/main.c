#include "time.h"
#include "utils.h"
#include "shader.h"

#define SCR_WIDTH	1500
#define SCR_HEIGHT	800

#define LEN(a)		sizeof((a)) / sizeof((a)[0])

int cells[SCR_WIDTH/10][SCR_HEIGHT/10];
int cell_buff[SCR_WIDTH/10][SCR_HEIGHT/10];

int ncx = SCR_WIDTH/10;
int ncy = SCR_HEIGHT/10;

float xsiz, ysiz, xgap, ygap;

unsigned int sp;
unsigned int vao;
int model_u;
int color_u;

double delta = 0;

void process_input(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, 1);
}

int main()
{
	void draw_cells();
	void calculate_cells();

	xsiz = 2.0f/ncx;
	ysiz = 2.0f/ncy;
	xgap = xsiz/10;
	ygap = ysiz/10;

	// generate random cells
	srand(time(NULL));
	for (int x = 0; x < ncx-1; x++)
		for (int y = 0; y < ncy-1; y++)
			// cell probability of 10%
			cells[x][y] = rand() < RAND_MAX / 10;

	GLFWwindow *window = get_window(SCR_WIDTH, SCR_HEIGHT);

	struct gl_color glc = hex_to_gl_color(0x003566);

	sp = build_shader("./shaders/simple.vs", "./shaders/simple.fs");
	model_u = glGetUniformLocation(sp, "model");
	color_u = glGetUniformLocation(sp, "color");

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	float vertices[] = {
		xgap,			ygap,		0,
		xgap,			ysiz - ygap,	0,
		xsiz - xgap,		ysiz - ygap,	0,
		xsiz - xgap,		ygap,		0,
	};

	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
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

	float calc_time = 0.5;
	float timer = 0;
	while (!glfwWindowShouldClose(window)) {
		double frame_start = glfwGetTime();

		glfwPollEvents();
		process_input(window);

		timer -= delta;
		if (timer <= 0) {
			timer = calc_time;
			calculate_cells();

			glClearColor(glc.R, glc.G, glc.B, 1.0f);
			glClearColor(0.3, 0.3, 0.3, 1.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			draw_cells();
			glfwSwapBuffers(window);

		}

		delta = glfwGetTime() - frame_start;
	}

	glfwTerminate();
	return 0;
}

void calculate_cells()
{
/*
    Any live cell with fewer than two live neighbours dies, as if by underpopulation.
    Any live cell with two or three live neighbours lives on to the next generation.
    Any live cell with more than three live neighbours dies, as if by overpopulation.
    Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
*/
	for (int x = 1; x < ncx-1; x++)
		for (int y = 1; y < ncy-1; y++)
			cell_buff[x][y] = cells[x][y];

	for (int x = 1; x < ncx-1; x++) {
		for (int y = 1; y < ncy-1; y++) {
			int n = 0;
			for (int dx = -1; dx <= 1; dx++)
				for (int dy = -1; dy <= 1; dy++)
					if (cells[x+dx][y+dy])
						n++;
			// since the loop counts the cell itself,
			// substract 1 if the cell was alive.
			n -= cells[x][y];

			if (cells[x][y]) {
				if (n < 2)
					cell_buff[x][y] = 0;
				if (n > 3)
					cell_buff[x][y] = 0;
			}
			else if (n == 3)
				cell_buff[x][y] = 1;
		}
	}

	for (int x = 1; x < ncx-1; x++)
		for (int y = 1; y < ncy-1; y++)
			cells[x][y] = cell_buff[x][y];
}

void draw_cells()
{
	float model[4][4] = {
		1,	0,	0,	0,
		0,	1,	0,	0,
		0,	0,	1,	0,
		0,	0,	0,	1,
	};

	glUseProgram(sp);
	glBindVertexArray(vao);
	for (int x = 1; x < ncx-1; x++) {
		for (int y = 1; y < ncy-1; y++) {
			model[3][0] = (ncx-1 - x) * xsiz - 1;
			model[3][1] = y * ysiz - 1;
			glUniform4f(color_u, cells[x][y], cells[x][y], cells[x][y], 1);
			glUniformMatrix4fv(model_u, 1, GL_FALSE, (float *) model);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
	}
}
