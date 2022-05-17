#ifndef CAMERA_H
#define CAMERA_H

#include "../deps/glm/glm.hpp"

class Camera {
public:
    Camera() = default;
    ~Camera() = default;

    /**
     * @brief  相机移动的向量
     * 
     * @param move_vec 
     */
    void move(const glm::vec3& move_vec);

    /**
     * @brief 向右移动
     * 
     * @param font_vec 向前的向量
     * @param speed 速度
     */
    void right(glm::vec3 font_vec, float speed);

    /**
     * @brief Get the view matrix 获取视图矩阵
     * 
     * @return glm::mat4 
     */
    glm::mat4 get_view();

private:
    glm::vec3 m_up = { 0.0f, 1.0f, 0.0f };  // 记录向上的向量
    glm::vec3 m_look_offset = { 0.0f, 0.0f, -1.0f};     // 相对相机 观察偏移矩阵
    glm::vec3 m_camera_pos = { 0.0f, 0.0f, 3.0f };
};

#endif // CAMERA_H