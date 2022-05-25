#ifndef CUBE_SPRITE_H
#define CUBE_SPRITE_H

#include <filesystem>
#include <memory>
#include <vector>
#include "sprite_object.h"
#include "texture2d.h"

#include "shader_load.hpp"

class CubeSprite : public SpriteObject{
public:
    CubeSprite(const std::vector<std::filesystem::path> & _vtfile);
    ~CubeSprite();

    virtual void draw(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) override;

private:
    std::vector<Texture2D> m_textures;  // 正方体六个面纹理
    std::shared_ptr<CShader> m_shader;
    
    uint32_t m_vbo;
    uint32_t m_vao;
};

#endif // CUBE_SPRITE_H