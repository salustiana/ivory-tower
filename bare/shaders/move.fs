#version 330 core
out vec4 frag_color;

//in vec3 pos;
in vec2 tex_coord;

uniform sampler2D texture_uni;
uniform sampler2D texture_uni2;

void main()
{
	frag_color = mix(texture(texture_uni, tex_coord), texture(texture_uni2, tex_coord), 1.0f);
}
