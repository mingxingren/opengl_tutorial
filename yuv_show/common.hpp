//
// Created by MMK on 2022/2/14.
//

#ifndef YUV_SHOW_COMMON_H
#define YUV_SHOW_COMMON_H

#include <glad/glad.h>
#include <iostream>

static void print_x_error() {
    while (true) {
        GLenum res = glGetError();
        if (res == GL_NO_ERROR) {
            std::cout << "##############print_opengl_error: " << res << std::endl;
        }
    }
}


#endif //YUV_SHOW_COMMON_H
