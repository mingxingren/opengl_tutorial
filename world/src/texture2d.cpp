#include "texture2d.h"
#include <string>
#include "common/macrodef.h"
#include "../deps/glad/glad.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../deps/stb/stb_image.h"

STD_NAMESPACE_DECLARE

Texture2D::Texture2D(int index, const filesystem::path& image_file, std::shared_ptr<CShader> _shader) 
    : m_index(index), Texture(_shader) {
    glGenTextures(1, &this->m_id);
    glBindTexture(GL_TEXTURE_2D, this->m_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);     // 反转 Y 向量
    unsigned char* data = stbi_load("res/cube_texture.jpg", &this->m_width, &this->m_height, &this->m_nrchannels, 0);
    if (data) {
        std::cout << "load texture success" << std::endl;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->m_width, this->m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    stbi_image_free(data);
}

Texture2D::~Texture2D() {
    glDeleteTextures(1, &this->m_id);
}

void Texture2D::bind() {
    string val_name = string("texture2d_") + to_string(this->m_index);
    this->m_shader->use();
    this->m_shader->set_int(val_name.c_str(), this->m_index);
    glActiveTexture(GL_TEXTURE0 + this->m_index);
    glBindTexture(GL_TEXTURE_2D, this->m_id);
}