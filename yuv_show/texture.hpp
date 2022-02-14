//
// Created by MMK on 2022/2/11.
//

#ifndef YUV_SHOW_TEXTURE_H
#define YUV_SHOW_TEXTURE_H

#include "common.hpp"

#define INVALID_TEXTURE 0

class CTexture {
public:
    CTexture(GLuint location) : shader_location(location){

    }

    ~CTexture() {

    }

    int location() {
        return  texture_id;
    }

    void render_texture(int w, int h, GLint internal, GLenum format, GLenum type, void* data) {
        if (w != this->width || h != this->height) {
            this->destroy_texture();    // 销毁之前的纹理
            glGenTextures(1, &this->texture_id);  // 生成纹理
            glBindTexture(GL_TEXTURE_2D, this->texture_id);   // 绑定纹理
            glTexImage2D(GL_TEXTURE_2D,
                         0,
                         GL_RGBA,
                         w,
                         h,
                         0,
                         GL_RGBA,
                         GL_UNSIGNED_BYTE,
                         NULL);     // 设置纹理, 此时像素数据填充设置为 NULL
            this->width = w;
            this->height = h;
        }

        this->update_texture(data);
    }

    void destroy_texture() {
        if (glIsTexture(this->texture_id) == GL_TRUE) {
            glDeleteTextures(1, &this->texture_id);
            this->texture_id = 0;
        }
    }

private:
    void update_texture(void * data) {
        glBindTexture(GL_TEXTURE_2D, this->texture_id);
        // UNPACK: cpu --> gpu
        // PACK: gpu --> cpu
        print_x_error();
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);  // 设置像素数据 从 CPU 到 GPU 的对齐方式, RGB为两字节对齐
        glPixelStorei(GL_UNPACK_ROW_LENGTH, this->width);    // 设置像素数据 从 CPU 到 GPU 的行的长度
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, this->width, this->height, GL_RGBA, GL_UNSIGNED_BYTE, data);

        glActiveTexture(GL_TEXTURE0 + this->shader_location);    // 激活 着色器的纹理单元, 置为活跃状态
        glBindTexture(GL_TEXTURE_2D, this->texture_id);   // 将 texture 与shader的纹理单元绑定
        // 设置图片放缩后
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

private:
    GLuint texture_id = INVALID_TEXTURE;   // 纹理单元坐标, 可以指定纹理值为 0 为未初始化
    GLuint shader_location;     // shader uniform纹理位置值
    int width = 0;
    int height = 0;
};

#endif //YUV_SHOW_TEXTURE_H
