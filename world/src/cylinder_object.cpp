#include "cylinder_object.h"
#include "common/common_func.h"
#include "../deps/glad/glad.h"
#include "shader_load.hpp"

CylinderObject::CylinderObject(unsigned int _w, unsigned int _h) : SpriteObject(), m_w(_w), m_h(_h) {
    this->m_shader = CShader::load_shader_width_path("shader/cylinder/cylinder.vs.glsl", "shader/cylinder/cylinder.fs.glsl", nullptr);;

    float hdirection_r = 1.0f;     // 算出水平方向半径
    float vdirection_h = 0.8f;       // 算出垂直方向高的一半
    double radian_vertical = atan(static_cast<float>(vdirection_h) / static_cast<float>(hdirection_r));         // 计算圆柱体弧度
    std::cout << "radian vertical: " << radian_vertical << std::endl;

    // 计算圆柱侧面所有点
    double radian_vertical_step = radian_vertical / this->m_vdirection_sample_count * 2;
    for (int y_index = 0; y_index < this->m_vdirection_sample_count; y_index++) {
        double y = static_cast<double>(hdirection_r ) * std::tan(radian_vertical - y_index * radian_vertical_step);
        for (int x_index = 0; x_index < this->m_hdirection_sample_count; x_index++){
            float xSegment = static_cast<float>(x_index) / static_cast<float>(this->m_hdirection_sample_count); 
            float x = std::cos(xSegment * 2.0f * G_PI) * hdirection_r;
            float z = std::sin(xSegment * 2.0f * G_PI) * hdirection_r;
            m_vtPoints.emplace_back(x, y, z);
        }
    }

    // 计算圆柱侧面三角形
    for (int y_index = 0; y_index < this->m_vdirection_sample_count + 1; y_index++) {
        for (int x_index = 0; x_index < this->m_hdirection_sample_count; x_index++) {
            m_vtTriangles.emplace_back(y_index * m_hdirection_sample_count + x_index);
            m_vtTriangles.emplace_back((y_index + 1) * m_hdirection_sample_count + x_index);
            m_vtTriangles.emplace_back((y_index + 1) * m_hdirection_sample_count + x_index + 1);
            m_vtTriangles.emplace_back(y_index * m_hdirection_sample_count + x_index);
            m_vtTriangles.emplace_back((y_index + 1) * m_hdirection_sample_count + x_index + 1);
            m_vtTriangles.emplace_back(y_index * m_hdirection_sample_count + x_index + 1);
        } 
    }

    glGenBuffers(1, &this->m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, this->m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_vtPoints.size() * sizeof(float) * 3, reinterpret_cast<void*>(&m_vtPoints[0]), GL_STATIC_DRAW);
    glGenBuffers(1, &this->m_veo);
    glGenVertexArrays(1, &this->m_vao);
    glBindVertexArray(this->m_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_veo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_vtTriangles.size() * sizeof(unsigned int), &this->m_vtTriangles[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);   
}

CylinderObject::~CylinderObject() {
    if (this->m_vao > 0) {
        glDeleteVertexArrays(1, &this->m_vao);
    }
    
    if (this->m_vbo > 0) {
        glDeleteBuffers(1, &this->m_vbo);
    }

    if (this->m_veo > 0) {
        glDeleteBuffers(1, &this->m_veo);
    }
}

void CylinderObject::draw(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) {
    this->m_shader->use();
    this->m_shader->set_matrix("model", model);
    this->m_shader->set_matrix("view", view);
    this->m_shader->set_matrix("projection", projection);
    glBindVertexArray(this->m_vao);
    glDrawElements(GL_TRIANGLES, this->m_vtTriangles.size(), GL_UNSIGNED_INT, 0);
    this->m_shader->unuse(); 
}