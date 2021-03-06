#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;        // 法线单位向量
out vec2 TexCoords;     // 漫反射贴图

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    FragPos = vec3(model * vec4(aPos, 1.0));    // 算出顶点通过平移和放缩后位置
    Normal = mat3(transpose(inverse(model))) * aNormal;     // 算出法线在物体放缩后实际法线
    gl_Position  = projection * view * vec4(FragPos, 1.0);
    TexCoords = aTexCoords;
}