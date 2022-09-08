#version 330 core

layout (location = 0) in vec3 a_pos;

uniform float lateral_disp;

out vec3 pos;

void main()
{
	gl_Position = vec4(a_pos.x + lateral_disp, -a_pos.y, a_pos.z, 1.0f);
	pos = vec3(gl_Position.x, gl_Position.y, gl_Position.z);
}
