#include "solid_light.h"
#include "../../../deps/glad/glad.h"
#include "../../model3d/cube_data.h"
#include "../../shader_load.hpp"

SolidLight::SolidLight(const glm::vec3 & color) : SpriteObject(), m_light_color(color) {
    glGenBuffers(1, &this->m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, this->m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(G_CUBE_DATA), reinterpret_cast<void*>(G_CUBE_DATA), GL_STATIC_DRAW);

    glGenVertexArrays(1, &this->m_vao);
    glBindVertexArray(this->m_vao);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);

    m_shader = CShader::load_shader_width_path("shader/cube/color_box.vs.glsl", "shader/cube/color_box.fs.glsl", nullptr);
    m_shader->use();
    m_shader->set_vec_4("plane_color", glm::vec4(this->m_light_color, 1.0f));
    m_shader->unuse();
}

SolidLight::~SolidLight() {

}

void SolidLight::draw(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) {
    m_shader->use();
    this->m_shader->set_matrix("model", model);
    this->m_shader->set_matrix("view", view);
    this->m_shader->set_matrix("projection", projection);
    glBindVertexArray(this->m_vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    m_shader->unuse();
}