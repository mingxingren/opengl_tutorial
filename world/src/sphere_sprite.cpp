#include "sphere_sprite.h" 

#include <vector>
#include "common/common_func.h"
#include "../deps/glad/glad.h"
#include "../deps/glm/glm.hpp"
#include "shader_load.hpp"

SphereSprite::SphereSprite() : SpriteObject() {
    this->m_shader = CShader::load_shader_width_path("shader/sphere/sphere.vs.glsl", "shader/sphere/sphere.fs.glsl", nullptr);

    glGenBuffers(1, &this->m_vbo);
    // glBindBuffer(GL_ARRAY_BUFFER, this->m_vbo);
    glGenBuffers(1, &this->m_ebo);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_ebo);
    glGenVertexArrays(1, &this->m_vao);
    // glBindVertexArray(this->m_vao);
    
    const unsigned int X_SEGMENTS = 64;
    const unsigned int Y_SEGMENTS = 64;
    for (unsigned int y = 0; y <= Y_SEGMENTS; y++) {
        for (unsigned int x = 0; x <= X_SEGMENTS; x++) {
            float xSegment = (float)x / (float)X_SEGMENTS;
            float ySegment = (float)y / (float)Y_SEGMENTS;
            float xPos = std::cos(xSegment * 2.0f * G_PI) * std::sin(ySegment * G_PI);
            float yPos = std::cos(ySegment * G_PI);
            float zPos = std::sin(xSegment * 2.0f * G_PI) * std::sin(ySegment * G_PI);
            this->m_vertices.emplace_back(xPos);
            this->m_vertices.emplace_back(yPos);
            this->m_vertices.emplace_back(zPos);
        }
    }
    
    std::vector<unsigned int> sphereIndices;
    for (int i = 0; i < Y_SEGMENTS; i++) {
        for (int j = 0; j < X_SEGMENTS; j++) {
            this->m_indices.push_back(i * (X_SEGMENTS + 1) + j);
            this->m_indices.push_back((i + 1) * (X_SEGMENTS + 1) + j);
            this->m_indices.push_back((i + 1) * (X_SEGMENTS + 1) + j + 1);
            this->m_indices.push_back(i * (X_SEGMENTS + 1) + j);
            this->m_indices.push_back((i + 1) * (X_SEGMENTS + 1) + j + 1);
            this->m_indices.push_back(i * (X_SEGMENTS + 1) + j + 1);
        }
    }

    glBindVertexArray(this->m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, this->m_vbo);
    glBufferData(GL_ARRAY_BUFFER, this->m_vertices.size() * sizeof(float),  &this->m_vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->m_indices.size() * sizeof(unsigned int), &this->m_indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);
}

SphereSprite::~SphereSprite() {
    if (this->m_vao > 0){
        glDeleteVertexArrays(1, &this->m_vao);
    }

    if (this->m_vbo > 0) {
        glDeleteBuffers(1, &this->m_vbo);
    }
    
    if (this->m_ebo > 0) {
        glDeleteBuffers(1, &this->m_ebo);
    }
}

void SphereSprite::draw(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) {
    this->m_shader->use();
    this->m_shader->set_matrix("model", model);
    this->m_shader->set_matrix("view", view);
    this->m_shader->set_matrix("projection", projection);

    glBindVertexArray(this->m_vao);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLES, this->m_x_segments * this->m_y_segments * 6, GL_UNSIGNED_INT, 0);
    this->m_shader->unuse(); 
}