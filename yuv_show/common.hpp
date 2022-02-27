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

            }
            break;
        }
        res = false;
    }
    return res;
}


#endif //YUV_SHOW_COMMON_H
