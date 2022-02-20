#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture sampler
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float diff;

void main()
{
	vec2 face_position = vec2(-2 * TexCoord.x, 2 * TexCoord.y);
	FragColor = mix(texture(texture1, 2 * TexCoord), texture(texture2, face_position), diff);
}