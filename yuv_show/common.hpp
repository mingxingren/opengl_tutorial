//
// Created by MMK on 2022/2/14.
//

#ifndef YUV_SHOW_COMMON_H
#define YUV_SHOW_COMMON_H

#include <glad/glad.h>
#include <iostream>

static bool print_x_error() {
    bool res = true;
    while (true) {
        GLenum err = glGetError();
        if (err == GL_NO_ERROR) {
            if (res) {
                std::cout << "##############print_opengl_error success" << std::endl;
            }
            break;
        }
        res = false;
        std::cout << "##############print_opengl_error: " << err << std::endl;
    }
    return res;
}


#endif //YUV_SHOW_COMMON_H
