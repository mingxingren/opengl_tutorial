#ifndef SPRITE_OBJECT_H
#define SPRITE_OBJECT_H

class SpriteObject {
public:
    SpriteObject() = default;
    ~SpriteObject() = default;

    /**
     * @brief draw
     * 
     * @param model 用于变换物体的位置 大小
     * @param view 观察者矩阵
     * @param projection 阴影矩阵
     */
    virtual void draw(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) = 0;
};

#endif // SPRITE_OBJECT_H