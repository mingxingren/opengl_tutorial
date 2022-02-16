//
// Created by MMK on 2021/12/28.
//
#pragma once

static char * fragment_content = R"(
#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

//format, filter, effect
uniform ivec4 icb;

// rgb 数据
uniform sampler2D rgb_texture;

// Yuv 数据
uniform sampler2D texY;
uniform sampler2D texU;
uniform sampler2D texV;

void yuv_to_rgb(float y, float u, float v, out vec4 rgba) {
    // Using "RGB to YCbCr color conversion for HDTV"
    y = (y - 0.0625) * 1.164;
    u = u - 0.5;
    v = v - 0.5;

    float r = y + 1.793 * v;
    float g = y - 0.213 * u - 0.533 * v;
    float b = y + 2.112 * u;

    rgba = vec4(r, g, b, 1.0);
}

void main()
{
    int format = icb[0];

    if (format == 2) {
        // NV12
        float y = texture2D(texY, TexCoord).r;
        float u = texture2D(texU, TexCoord).r;
        float v = texture2D(texV, TexCoord).g;
        yuv_to_rgb(y, u, v, FragColor);
    }
    else if (format == 3 || format == 4) {
        // I420
        float y = texture2D(texY, TexCoord).r;
        float u = texture2D(texU, TexCoord).r;
        float v = texture2D(texV, TexCoord).r;

        yuv_to_rgb(y, u, v, FragColor);
    }else {
        FragColor = texture2D(rgb_texture, TexCoord);
    }
}
)";

