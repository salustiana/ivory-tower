#include "cglm/vec3.h"
#include "cglm/cam.h"

struct camera {
	float pos[3];
	float dir[3];
	float  up[3];
	float left[3];
	float aim[3];
};

void set_cam(struct camera *cam, float pos[3], float dir[3], float up[3]);

void set_view(struct camera *cam, mat4 view);

void translate_cam(struct camera *cam, float dir[3], float dist);

void rotate_cam(struct camera *cam, float center[3], float axis[3], float rads);

void spin_cam(struct camera *cam, float axis[3], float rads);
