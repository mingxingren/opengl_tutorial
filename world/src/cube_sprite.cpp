#include "cube_sprite.h"

#include <cassert>
#include "../deps/stb/stb_image.h"
#include "../deps/glad/glad.h"

#define CUBE_SURFACE_NUM 6

CubeSprite::CubeSprite(const std::vector<std::filesystem::path> & _vtfile) {
    assert((_vtfile.size() >= CUBE_SURFACE_NUM));

    this->m_shader = CShader::load_shader_width_path("shader/cube/cube.vs.glsl", "shader/cube/cube.fs.glsl", nullptr);
    for (int i = 0; i < CUBE_SURFACE_NUM; i ++) {
        this->m_textures.emplace_back(i, _vtfile[i], this->m_shader);
    }

    float cube_vertexs[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    glGenBuffers(1, &this->m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, this->m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertexs), cube_vertexs, GL_STATIC_DRAW);

    glGenVertexArrays(1, &this->m_vao);
    glBindVertexArray(this->m_vao);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), cube_vertexs);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), cube_vertexs + sizeof(float) * 3);
}

CubeSprite::~CubeSprite() {
    glDeleteVertexArrays(1, &this->m_vao);
    glDeleteBuffers(1, &this->m_vbo);
}

void CubeSprite::draw(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) {
    this->m_shader->use();
    for (int i = 0; i < CUBE_SURFACE_NUM; i++) {
        this->m_textures[i].bind();
    }

    glBindVertexArray(this->m_vao);
    this->m_shader->set_matrix("model", model);
    this->m_shader->set_matrix("view", view);
    this->m_shader->set_matrix("projection", projection);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}