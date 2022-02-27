//
// Created by MMK on 2021/12/28.
//
#pragma once

static char * vertex_content = R"(
attribute vec4 position;
attribute vec2 texcoor;

varying vec2 TexCoord;

void main()
{
	gl_Position = position;
	TexCoord = texcoor;
})";
