//
// Created by MMK on 2022/2/11.
//

#ifndef YUV_SHOW_TEXTURE_H
#define YUV_SHOW_TEXTURE_H

#include "common.hpp"

#define INVALID_TEXTURE 0

class CTexture {
public:
    CTexture(GLuint rgb_location, GLuint y_location, GLuint u_location, GLuint v_location)
    : shader_rgb_location(rgb_location)
    , shader_y_location(y_location)
    , shader_u_location(u_location)
    , shader_v_location(v_location){

    }

    ~CTexture() {
        this->destroy_texture();
    }

    void render_texture(int w, int h, GLint internal, GLenum format, GLenum type, void* data) {
        if (w != this->width || h != this->height) {
            this->destroy_texture();    // 销毁之前的纹理
            glGenTextures(1, &this->texture_rgb_id);  // 生成纹理
            glBindTexture(GL_TEXTURE_2D, this->texture_rgb_id);   // 绑定纹理
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

    void render_I420_texture(int w, int h, const void* y_data, const void *u_data, const void *v_data) {
        if (w != this->width || h != this->height) {
            this->destroy_texture();

            // Y 分量纹理
            glGenTextures(1, &this->texture_y);
            glBindTexture(GL_TEXTURE_2D, this->texture_y);
            // GL_R8 指定纹理颜色格式为 R: 1, G: 0, B: 0 A: 1
            // GL_RED 指定cpu 像素数据为 R分量字符串, 此处可以指为 Y
            glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);

            // U 分量纹理
            glGenTextures(1, &this->texture_u);
            glBindTexture(GL_TEXTURE_2D, this->texture_u);
            // GL_R8 指定纹理颜色格式为 R: 1, G: 0, B: 0 A: 1
            // GL_RED 指定cpu 像素数据为 R分量字符串, 此处可以指为 Y
            // U 分量像素长度 为Y分量像素的四分之一, 即: (w/2) * (h/2)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, w / 2, h / 2 , 0, GL_RED, GL_UNSIGNED_BYTE, NULL);

            // V 分量纹理
            glGenTextures(1, &this->texture_v);
            glBindTexture(GL_TEXTURE_2D, this->texture_v);
            // GL_R8 指定纹理颜色格式为 R: 1, G: 0, B: 0 A: 1
            // GL_RED 指定cpu 像素数据为 R分量字符串, 此处可以指为 Y
            // V 分量像素长度 为Y分量像素的四分之一, 即: (w/2) * (h/2)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, w / 2, h / 2, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);

            this->width = w;
            this->height = h;
        }

        this->update_yuv_planar(this->texture_y, this->shader_y_location, y_data, this->width, this->height);
        this->update_yuv_planar(this->texture_u, this->shader_u_location, u_data, this->width / 2, this->height / 2);
        print_x_error();
        this->update_yuv_planar(this->texture_v, this->shader_v_location, v_data, this->width / 2, this->height / 2);
    }

    void destroy_texture() {
        if (glIsTexture(this->texture_rgb_id) == GL_TRUE) {
            glDeleteTextures(1, &this->texture_rgb_id);
            this->texture_rgb_id = 0;
        }

        if (glIsTexture(this->texture_y) == GL_TRUE) {
            glDeleteTextures(1, &this->texture_y);
        }

        if (glIsTexture(this->texture_u) == GL_TRUE) {
            glDeleteTextures(1, &this->texture_u);
        }

        if (glIsTexture(this->texture_v) == GL_TRUE) {
            glDeleteTextures(1, &this->texture_v);
        }
    }

private:
    void update_texture(void * data) {
        glBindTexture(GL_TEXTURE_2D, this->texture_rgb_id);
        // UNPACK: cpu --> gpu
        // PACK: gpu --> cpu
        print_x_error();
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);  // 设置像素数据 从 CPU 到 GPU 的对齐方式, RGB为两字节对齐
        glPixelStorei(GL_UNPACK_ROW_LENGTH, this->width);    // 设置像素数据 从 CPU 到 GPU 的行的长度
        std::cout << "#################width: " << this->width << " height: " << this->height << std::endl;
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, this->width, this->height, GL_RGBA, GL_UNSIGNED_BYTE, data);

        glActiveTexture(GL_TEXTURE0 + this->shader_rgb_location);    // 激活 着色器的纹理单元, 置为活跃状态
        glBindTexture(GL_TEXTURE_2D, this->texture_rgb_id);   // 将 texture 与 当前活跃的shader纹理单元绑定
        // 设置图片放缩后
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    void update_yuv_planar(GLuint texture_id, GLuint shader_location, const void *data, GLint width, GLint height) {
        glBindTexture(GL_TEXTURE_2D, texture_id);
        glPixelStorei(GL_UNPACK_ROW_LENGTH, width);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RED, GL_UNSIGNED_BYTE, data);
        glActiveTexture(GL_TEXTURE0 + shader_location);

        glBindTexture(GL_TEXTURE_2D, texture_id);   // 将 texture 与 当前活跃的shader纹理单元绑定
        // 设置图片放缩后
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

private:
    GLuint texture_rgb_id = INVALID_TEXTURE;   // rgb纹理单元坐标, 可以指定纹理值为 0 为未初始化
    GLuint texture_y = INVALID_TEXTURE;    // YUV y分量 纹理单元坐标, 可以指定纹理值为 0 为未初始化
    GLuint texture_u = INVALID_TEXTURE;    // YUV u分量 纹理单元坐标, 可以指定纹理值为 0 为未初始化
    GLuint texture_v = INVALID_TEXTURE;    // YUV v分量 纹理单元坐标, 可以指定纹理值为 0 为未初始化
    GLuint shader_rgb_location;     // shader uniform纹理位置值
    GLuint shader_y_location;       // uniform y 纹理位置值
    GLuint shader_u_location;       // uniform u 纹理位置值
    GLuint shader_v_location;       // uniform v 纹理位置值
    int width = 0;
    int height = 0;
};

#endif //YUV_SHOW_TEXTURE_H
