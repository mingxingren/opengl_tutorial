//
// Created by MMK on 2021/12/30.
//

#ifndef YUV_SHOW_PAINTER_H
#define YUV_SHOW_PAINTER_H

#include <functional>
#include <atomic>
#include "SDL.h"
#include "glad/glad.h"
#include "shaderload.hpp"
#include "texture.hpp"

class CPainter{
public:
    using type_swap_window = std::function<void(void*)>;
    using type_make_current = std::function<void(void*, void*)>;

private:
    type_swap_window fn_swap_window = nullptr;
    type_make_current fn_make_current = nullptr;
    std::unique_ptr<CTexture> video_texture = nullptr;

public:
    CPainter(void * window, void * glcontext, int dlg_w, int dlg_h)
    : dialog_width(dlg_w), dialog_height(dlg_h){
        this->window = window;
        this->glcontext = glcontext;
        this->rgb_location_value = 0;   // 对应 Shader 纹理 GL_TEXTURE0
        this->y_location_value = 1;     // 对应 Shader 纹理 GL_TEXTURE1
        this->u_location_value = 2;     // 对应 Shader 纹理 GL_TEXTURE2
        this->v_location_value = 3;     // 对应 Shader 纹理 GL_TEXTURE3

        if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) {
            std::cerr << "gladLoadGLLoader call fail" << std::endl;
        }else{
            std::cout << "gladLoadGLLoader call success" << std::endl;
        }

        this->shader_load =  std::make_shared<CShaderload>();

        // 申请顶点缓冲队列
        glGenVertexArrays(1, &this->VAO);
        // 申请两块内存
        glGenBuffers(1, &this->VBO);    // 顶点内存对象
        glGenBuffers(1, &this->EBO);    // 顶点索引对象

        {
            glBindVertexArray(this->VAO);   // 绑定VAO 将当前操作对象指向VAO

            glBindBuffer(GL_ARRAY_BUFFER, this->VBO);   // 将当前要操作的顶点内存对象指向 VBO
            glBufferData(GL_ARRAY_BUFFER, sizeof(this->space_vertices), space_vertices, GL_STATIC_DRAW);    // 填充 VBO 数据

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);   // 将绑定EBO 绑定到VAO
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);    // 填充EBO

            // 将VAO中的顶点坐标数据绑定到 location 0 变量
            // 将当前操作的顶点缓冲绑定到 VAO上, 并指定如何解析该顶点内存对象
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
            // 激活 location 0 变量
            glEnableVertexAttribArray(0);

            // 将VAO中的纹理坐标数据绑定到 location 1 变量
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
            // 激活 location 1 变量
            glEnableVertexAttribArray(1);

            glBindVertexArray(0);
        }

        this->shader_load->Use();
        this->shader_load->SetInt("rgb_texture", this->rgb_location_value);
        this->shader_load->SetInt("texY", this->y_location_value);
        this->shader_load->SetInt("texU", this->u_location_value);
        this->shader_load->SetInt("texV", this->v_location_value);
        this->shader_load->SetVec4("icb", 2, 0, 0, 1);
        this->video_texture = std::make_unique<CTexture>(this->rgb_location_value,
                                                         this->y_location_value,
                                                         this->u_location_value,
                                                         this->v_location_value);

        // 设置 窗口输出尺寸
        glViewport(0, 0, this->dialog_width, this->dialog_height);
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
        // 设置清空的颜色
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        this->video_texture->render_texture(width, height, 0, 0, 0, data);

        this->shader_load->Use();
        glBindVertexArray(this->VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
        fn_swap_window(this->window);
    }

    void PainterYUV(int width, int height,
                    const unsigned char* y_planar,
                    const unsigned char* u_planar,
                    const unsigned char* v_planar) {
        // 设置清空的颜色
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        this->video_texture->render_I420_texture(width, height, y_planar, u_planar, v_planar);

        this->shader_load->Use();
        glBindVertexArray(this->VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
        fn_swap_window(this->window);
    }

    void PainterNV(int width, int height,
                    const unsigned char* y_planar,
                    const unsigned char* uv_planar) {
        // 设置清空的颜色
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        this->video_texture->render_nv12_texture(width, height, y_planar, uv_planar);

        this->shader_load->Use();
        glBindVertexArray(this->VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
        fn_swap_window(this->window);
    }

    void Viewport(int x, int y, int width, int height) {
        glViewport(x, y, width, height);
    }

    void MakeCurrentContext() {
        fn_make_current(window, glcontext);
    }

    void * GetWindow() {
        return this->window;
    }

private:
    float space_vertices[20] = {
            // 顶点坐标            // 纹理坐标
            -1.0f, 1.0f, 0.0f,    0.0f, 0.0f, // 顶点坐标: 左上   纹理坐标: 左下
            -1.0f, -1.0f, 0.0f,   0.0f, 1.0f, // 顶点坐标: 左下   纹理坐标: 左上
            1.0f, -1.0f, 0.0f,  1.0f, 1.0f,  // 顶点坐标: 右下    纹理坐标: 右上
            1.0f, 1.0f, 0.0f,   1.0f, 0.0f, // 顶点坐标: 右上     纹理坐标: 右下
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
    int rgb_location_value;
    int y_location_value;
    int u_location_value;
    int v_location_value;
    std::shared_ptr<CShaderload> shader_load = nullptr;

    void* window;
    void* glcontext;
    std::atomic_int32_t  dialog_width;
    std::atomic_int32_t  dialog_height;
};

#endif //YUV_SHOW_PAINTER_H
