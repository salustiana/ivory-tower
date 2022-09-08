#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_col;
layout (location = 2) in vec2 a_tex_coord;

uniform float lateral_disp;

out vec3 pos;
out vec2 tex_coord;

void main()
{
	gl_Position = vec4(a_pos.x + lateral_disp, a_pos.y, a_pos.z, 1.0f);
	pos = vec3(gl_Position.x, gl_Position.y, gl_Position.z);
	tex_coord = a_tex_coord;
}
