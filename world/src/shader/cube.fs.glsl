#version 330 core
in vec2 TexCoords;
out vec4 FragColor;

uniform sample2D texture2d_1;

void main() {
    FragColor = texture(texture2d_1, TexCoords);
}