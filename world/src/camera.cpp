#include "camera.h"
#include "../deps/glm/gtc/matrix_transform.hpp"

void Camera::move(float speed) {
    // 计算相机现在的位置
    this->m_camera_pos = this->m_camera_pos + speed * this->m_front;
}

void Camera::right(float speed) {
    // 欧拉角向量指由屏幕内指向屏幕外 叉乘向上的向量 得到方向是像左的
    this->m_camera_pos -= glm::normalize(glm::cross(this->m_front, this->m_up)) * speed;
}

glm::mat4 Camera::get_view() {
    return glm::lookAt(this->m_camera_pos, this->m_camera_pos + this->m_front, this->m_up);
}

void Camera::set_front(glm::vec3 front_vec) {
    this->m_front = front_vec;
    // 利用y轴单元向量 和 front 向量 计算 向右的向量 和 相机实际的向上向量
    glm::vec3 up = { 0.0f, 1.0f, 0.0f };
    glm::vec3 Right = glm::normalize(glm::cross(this->m_front, up));
    this->m_up = glm::cross(Right, this->m_front);
    this->m_up = glm::normalize(this->m_up);
}