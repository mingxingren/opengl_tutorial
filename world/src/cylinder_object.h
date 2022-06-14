#ifndef CYLINDER_H
#define CYLINDER_H

#include <memory>
#include <vector>
#include "sprite_object.h"
#include "../deps/glm/glm.hpp"

class CShader;
class CylinderObject : public SpriteObject {
public:
    CylinderObject(unsigned int _w, unsigned int _h);
    ~CylinderObject();

    /**
     * @brief draw
     * 
     * @param model 用于变换物体的位置 大小
     * @param view 观察者矩阵
     * @param projection 阴影矩阵
     */
    virtual void draw(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection);

private:
    std::shared_ptr<CShader> m_shader = nullptr;
    unsigned int m_w;       ///< 宽的比例
    unsigned int m_h;       ///< 高的比例
    unsigned int m_vdirection_sample_count = 64;      ///< 垂直方向采样数量
    unsigned int m_hdirection_sample_count = 32;      ///< 水平方向采样数量
    
    unsigned int m_vao;
    unsigned int m_vbo;
    unsigned int m_veo;

    std::vector<glm::vec3> m_vtPoints;
    std::vector<float> m_vtPoints_copy;
    std::vector<unsigned int> m_vtTriangles;
};

#endif // CYLINDER_H