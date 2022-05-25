#version 330 core
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTexCoords;

out vec2 TexCoords;
out int surface_number;

# 投影矩阵
uniform mat4 projection;
# 视图矩阵
uniform mat4 view;
uniform mat4 model;

void main() {
	if (surface_number.z == 0.5f) {
		surface_number = 1;
	}
	else if (surface_number.y == 0.5f) {
		surface_number = 2;
	}
	else if (surface_number.z == -0.5f) {
		surface_number = 3;
	}
	else if (surface_number.y == -0.5f) {
		surface_number = 4;
	}
	else if (surface_number.x == -0.5f) {
		surface_number = 5;
	}
	else if (surface_number.x == 0.5f) {
		surface_number = 6;
	}

    TexCoords = aTexCoords;
    gl_Position = projection * view * model * vec4(aPosition, 1.0f);
}