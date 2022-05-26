#ifndef BOX_SPRITE_H
#define BOX_SPRITE_H

#include <filesystem>
#include "sprite_object.h"

class BoxSprite : public SpriteObject {
public:
    BoxSprite(const std::filesystem::path & _file);
    ~BoxSprite();

    virtual void draw(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) = 0;
};

#endif  // BOX_SPRITE_H