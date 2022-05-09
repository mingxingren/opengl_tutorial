#ifndef TEXTURE_H
#define TEXTURE_H

#include <memory>
#include "shader_load.hpp"

class Texture{
public:
    Texture(std::shared_ptr<CShader> _shader) : m_shader(_shader) {}
    virtual ~Texture() = default;

    virtual void bind() = 0;

protected:
    std::shared_ptr<CShader> m_shader;
};

#endif // TEXTURE_H