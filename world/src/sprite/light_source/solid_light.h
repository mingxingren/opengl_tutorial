#include "../../sprite_object.h"
#include <memory>
#include "../../../deps/glm/glm.hpp"

class CShader;

class SolidLight : public SpriteObject {
public:
    SolidLight(const glm::vec3 & color);
    ~SolidLight();

    /**
     * @brief draw
     * 
     * @param model 用于变换物体的位置 大小
     * @param view 观察者矩阵
     * @param projection 阴影矩阵
     */
    virtual void draw(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection);

private:
    unsigned int m_vao;
    unsigned int m_vbo;

    glm::vec3 m_light_color;    // 光照
    std::shared_ptr<CShader> m_shader = nullptr;
};