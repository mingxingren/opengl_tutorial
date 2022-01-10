//
// Created by MMK on 2021/12/30.
//

#ifndef YUV_SHOW_PAINTER_H
#define YUV_SHOW_PAINTER_H

#include <functional>
#include "SDL.h"
#include "glad/glad.h"
#include "shaderload.hpp"

class CPainter{
public:
    using type_swap_window = std::function<void(void*)>;
    using type_make_current = std::function<void(void*, void*)>;

private:
    type_swap_window fn_swap_window = nullptr;
    type_make_current fn_make_current = nullptr;

public:
    CPainter(void * window, void * glcontext) {
        this->window = window;
        this->glcontext = glcontext;

        if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) {
            std::cerr << "gladLoadGLLoader call fail" << std::endl;
        }else{
            std::cout << "gladLoadGLLoader call success" << std::endl;
        }

        this->shader_load =  std::make_shared<CShaderload>();

        // 申请顶点缓冲队列
        glGenVertexArrays(1, &this->VAO);
        // 申请两块内存
        glGenBuffers(1, &this->VBO);
        glGenBuffers(1, &this->EBO);

        {
            glBindVertexArray(this->VAO);

            glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(this->space_vertices), space_vertices, GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

            // 将数据绑定到 location 0 变量
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
            // 激活 location 0 变量
            glEnableVertexAttribArray(0);

            // 将数据绑定到 location 1 变量
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
            // 激活 location 1 变量
            glEnableVertexAttribArray(1);

            glBindVertexArray(0);
        }

        {
            glGenTextures(1, &this->texture);
            glBindTexture(GL_TEXTURE_2D, this->texture);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        }

        this->shader_load->Use();
        this->shader_load->SetInt("rgb_texture", 0);
    }

    ~CPainter() {
        glDeleteVertexArrays(1, &this->VAO);
        glDeleteBuffers(1, &this->VBO);
        glDeleteBuffers(1, &this->EBO);
        glDeleteTextures(1, &this->texture);
    }

    void RegisterSwapWindowFunc(type_swap_window func) {
        fn_swap_window = func;
    }

    void RegisterMakeCurrentFunc(type_make_current func) {
        fn_make_current = func;
    }

    void Painter(unsigned char* data, size_t data_size, int width, int height) {
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindTexture(GL_TEXTURE_2D, this->texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_INT, data);
        glBindTexture(GL_TEXTURE_2D, 0);

        this->shader_load->Use();
        glBindVertexArray(this->VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
        fn_swap_window(window);
    }

    void Viewport(int x, int y, int width, int height) {
        glViewport(x, y, width, height);
    }

    void MakeCurrentContext() {
        fn_make_current(window, glcontext);
    }

private:
    float space_vertices[20] = {
            // 顶点坐标            // 纹理坐标
            1.0f, 1.0f, 0.0f,    1.0f, 1.0f, // 右上
            1.0f, -1.0f, 0.0f,   1.0f, 0.0f, // 右下
            -1.0f, -1.0f, 0.0f,  0.0f, 0.0f, // 左下
            -1.0f, 1.0f, 0.0f,   0.0f, 1.0f, // 左上
    };

    // 顶点索引坐标
    unsigned int indices[6] = {
            0, 1, 3,
            1, 2, 3
    };

    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    GLuint texture;
    std::shared_ptr<CShaderload> shader_load = nullptr;

    void* window;
    void* glcontext;
};

#endif //YUV_SHOW_PAINTER_H
