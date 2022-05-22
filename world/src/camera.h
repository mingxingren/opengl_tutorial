#ifndef CAMERA_H
#define CAMERA_H

#include "../deps/glm/glm.hpp"

class Camera {
public:
    Camera() = default;
    ~Camera() = default;

    /**
     * @brief  相机向前后移动
     * 
     * @param speed  带有方向的速度
     */
    void move(float speed);

    /**
     * @brief 向右移动
     * 
     * @param speed 速度
     */
    void right(float speed);

    /**
     * @brief Get the view matrix 获取视图矩阵
     * 
     * @return glm::mat4 
     */
    glm::mat4 get_view();

    /**
     * @brief Set the front object 设置摄像机角度向量
     * 
     * @param front_vec 
     */
    void set_front(glm::vec3 front_vec);

private:
    glm::vec3 m_up = { 0.0f, 1.0f, 0.0f };  // 记录向上的向量
    // glm::vec3 m_look_offset = { 0.0f, 0.0f, -1.0f};     // 相对相机 观察偏移矩阵
    glm::vec3 m_camera_pos = { 0.0f, 0.0f, 3.0f };
    glm::vec3 m_front = { 0.0f, 0.0f, -1.0f };
};

#endif // CAMERA_H