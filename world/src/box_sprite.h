#ifndef BOX_SPRITE_H
#define BOX_SPRITE_H

#include <memory>
#include <filesystem>
#include "sprite_object.h"
#include "texture2d.h"

class BoxSprite : public SpriteObject {
public:
    BoxSprite(const std::filesystem::path & _file);
    ~BoxSprite();

    virtual void draw(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection);

private:
    std::shared_ptr<CShader> m_shader;
    std::unique_ptr<Texture2D> m_texture;

    unsigned int m_vao;
    unsigned int m_vbo;
    unsigned int m_texture_index;
};

#endif  // BOX_SPRITE_H