#ifndef SPHERE_SPRITE_H
#define SPHERE_SPRITE_H

#include <memory>
#include <vector>
#include "sprite_object.h"

class CShader;

class SphereSprite : public SpriteObject {
public:
    SphereSprite();
    ~SphereSprite();

    /**
     * @brief draw
     * 
     * @param model 用于变换物体的位置 大小
     * @param view 观察者矩阵
     * @param projection 阴影矩阵
     */
    virtual void draw(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) override;

private:
    unsigned int m_vao = -1;
    unsigned int m_ebo = -1;
    unsigned int m_vbo = -1;
    unsigned int m_x_segments = 64;
    unsigned int m_y_segments = 64;

    std::vector<float> m_vertices;
    std::vector<int> m_indices;
    std::shared_ptr<CShader> m_shader;
};

#endif //SPHERE_SPRITE_H