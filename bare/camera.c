#include "camera.h"

void update_cam(struct camera *cam)
{
	for (int i = 0; i < 3; i++)
		cam->aim[i] = cam->pos[i] + cam->dir[i];
	glm_cross(cam->up, cam->dir, cam->left);
}

void set_cam(struct camera *cam, float pos[3], float dir[3], float up[3])
{
	for (int i = 0; i < 3; i++) {
		cam->pos[i] = pos[i];
		cam->dir[i] = dir[i];
		cam->up[i] = up[i];
	}
	update_cam(cam);
}

void set_view(struct camera *cam, mat4 view)
{
	glm_lookat(cam->pos, cam->aim, cam->up, view);
}

void translate_cam(struct camera *cam, float dir[3], float dist)
{
	for (int i = 0; i < 3; i++) {
		cam->pos[i] += dist * dir[i];
		cam->aim[i] = cam->pos[i] + cam->dir[i];
	}
}

void rotate_cam(struct camera *cam, float center[3], float axis[3], float rads)
{
	float vec[3];
	for (int i = 0; i < 3; i++)
		vec[i] = cam->pos[i] - center[i];

	glm_vec3_rotate(vec, rads, axis);
	for (int i = 0; i < 3; i++)
		cam->pos[i] = vec[i];
	glm_vec3_rotate(cam->dir, rads, axis);
	glm_vec3_rotate(cam->up, rads, axis);

	update_cam(cam);
}

void spin_cam(struct camera *cam, float axis[3], float rads)
{
	glm_vec3_rotate(cam->dir, rads, axis);
	glm_vec3_rotate(cam->up, rads, axis);
	update_cam(cam);
}

void set_cam_dir(struct camera *cam, float dir[3])
{
	cam->dir[0] = dir[0];
	cam->dir[1] = dir[1];
	cam->dir[2] = dir[2];
	update_cam(cam);
}
