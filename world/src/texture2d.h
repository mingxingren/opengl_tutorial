#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include "texture.h"
#include <filesystem>
#include <memory>

class Texture2D : public Texture {
public:
    Texture2D(int index, const std::filesystem::path & image_file, std::shared_ptr<CShader> _shader);
    virtual ~Texture2D();
    virtual void bind() override;

private:
    unsigned int m_id;
    int m_width;
    int m_height;
    int m_nrchannels;
    int m_index;
};

#endif // TEXTURE2D_H