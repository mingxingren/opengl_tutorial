//
// Created by MMK on 2021/12/28.
//
#pragma once

static char * fragment_content = R"(
#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
uniform sampler2D rgb_texture;

void main()
{
	FragColor = texture(rgb_texture, TexCoord);
}
)";

