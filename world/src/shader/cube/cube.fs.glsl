#version 330 core
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D texture2d_0;
uniform sampler2D texture2d_1;
uniform sampler2D texture2d_2;
uniform sampler2D texture2d_3;
uniform sampler2D texture2d_4;
uniform sampler2D texture2d_5;

uniform int surface_number;

void main() {
	if (surface_number == 1) {
		FragColor = texture(texture2d_0, TexCoords);
	}
	else if (surface_number == 2) {
		FragColor = texture(texture2d_1, TexCoords);
	}
	else if (surface_number == 3) {
		FragColor = texture(texture2d_2, TexCoords);
	}
	else if (surface_number == 4) {
		FragColor = texture(texture2d_3, TexCoords);
	}
	else if (surface_number == 5) {
		FragColor = texture(texture2d_4, TexCoords);
	}
	else if (surface_number == 6) {
		FragColor = texture(texture2d_5, TexCoords);
	}

}