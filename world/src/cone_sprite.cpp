#include "cone_sprite.h"
#include "../deps/glad/glad.h"
#include "../deps/glm/glm.hpp"
#include "common/common_func.h"
#include "shader_load.hpp"

ConeSprite::ConeSprite() : SpriteObject() {
    this->m_shader = CShader::load_shader_width_path("shader/cone/cone.vs.glsl", "shader/cone/cone.fs.glsl", nullptr);
    float y = -0.5f;
    float r = 0.5f;

    std::vector<glm::vec3> vtPoints;
    vtPoints.emplace_back(0.0f, 0.5f, 0.0f);
    for (int index = 0; index < this->m_circle_sample_count; index++) {
        float segment = static_cast<float>(index) / this->m_circle_sample_count;
        float x = r * std::cos(segment * 2.0 * G_PI);
        float z = r * std::sin(segment * 2.0 * G_PI);
        vtPoints.emplace_back(x, y, z);
    }

    for (int index = 1; index < this->m_circle_sample_count; index++) {
        this->m_elements.push_back(0);
        this->m_elements.push_back(index);
        this->m_elements.push_back(index + 1);
    }
    this->m_elements.push_back(0);
    this->m_elements.push_back(this->m_circle_sample_count);
    this->m_elements.push_back(1);
    

    glGenBuffers(1, &this->m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, this->m_vbo);
    glBufferData(GL_ARRAY_BUFFER, vtPoints.size() * sizeof(float) * 3, &vtPoints[0], GL_STATIC_DRAW);
    
    glGenVertexArrays(1, &this->m_vao);
    glBindVertexArray(this->m_vao);
    
    glGenBuffers(1, &this->m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->m_elements.size() * sizeof(unsigned int) * 2, &this->m_elements[0], GL_STATIC_DRAW);
    
    this->m_shader->use();
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);
}

ConeSprite::~ConeSprite() {

}

void ConeSprite::draw(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) {
    this->m_shader->use();
    this->m_shader->set_matrix("model", model);
    this->m_shader->set_matrix("view", view);
    this->m_shader->set_matrix("projection", projection);
    glBindVertexArray(this->m_vao);
    glDrawElements(GL_TRIANGLES, this->m_elements.size(), GL_UNSIGNED_INT, reinterpret_cast<void*>(0));
    this->m_shader->unuse();
}