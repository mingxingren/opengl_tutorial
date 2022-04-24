#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;

out vec2 TexCoord; 

uniform mat4 mode;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	TexCoord = aTexCoord;
}