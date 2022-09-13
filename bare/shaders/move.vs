#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_tex_coord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//out vec3 pos;
out vec2 tex_coord;

void main()
{
	tex_coord = a_tex_coord;
	//gl_Position = projection * view * model * vec4(a_pos, 1.0f);
	gl_Position = projection * view * model * vec4(a_pos, 1.0);
}
