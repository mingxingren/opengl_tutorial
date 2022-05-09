#version 330 core
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

# 投影矩阵
uniform mat4 projection;
# 视图矩阵
uniform mat4 view;
uniform mat4 model;

void main() {
    TexCoords = aTexCoords;
    gl_Position = projection * view * model * vec4(aPosition, 1.0f);
}