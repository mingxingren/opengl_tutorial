#version 330 core
layout(location = 0) in vec3 aPosition;

# 投影矩阵
uniform mat4 projection;
# 视图矩阵
uniform mat4 view;
uniform mat4 model;

void main() {
    gl_Position = projection * view * model * vec4(aPosition, 1.0f);
}