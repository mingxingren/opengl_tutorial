#ifndef CONE_SPRITE_H
#define CONE_SPRITE_H

#include <memory>
#include <vector>
#include "sprite_object.h"

class CShader;
class ConeSprite : public SpriteObject {
public:
    ConeSprite();
    ~ConeSprite();

    void draw(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection);

private:
    unsigned int m_vao = 0;
    unsigned int m_vbo = 0;
    unsigned int m_ebo = 0;

    unsigned int m_circle_sample_count = 64;    // 圆锥地盘采样点的采样数
    std::vector<int> m_elements;    // 索引
    std::shared_ptr<CShader> m_shader = nullptr;
};

#endif // CONE_SPRITE_H