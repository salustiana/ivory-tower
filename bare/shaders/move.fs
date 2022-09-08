#version 330 core
out vec4 frag_color;

in vec3 pos;
in vec2 tex_coord;

uniform sampler2D texture_uni;

void main()
{
	frag_color = texture(texture_uni, tex_coord);
}
