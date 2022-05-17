#include "camera.h"
#include "../deps/glm/gtc/matrix_transform.hpp"

void Camera::move(const glm::vec3& move_vec) {
    // 计算相机现在的位置
    this->m_camera_pos = this->m_camera_pos + move_vec;
}

void Camera::right(glm::vec3 font_vec, float speed) {
    this->m_camera_pos -= glm::normalize(glm::cross(font_vec, this->m_up)) * speed;
}

glm::mat4 Camera::get_view() {
    return glm::lookAt(this->m_camera_pos, this->m_camera_pos + this->m_look_offset, this->m_up);
}