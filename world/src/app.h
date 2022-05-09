#pragma once

#include <mutex>
#include "../deps/glad/glad.h"
#include "GLFW/glfw3.h"

class App {
public:
    explicit App();
    App(const App&) = delete;
    ~App();
    void run();

private:
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    static void processInput(GLFWwindow* window);

private:
    static std::once_flag g_init_flag;
    
    int m_dialog_width = 800;
    int m_dialog_height = 600;

    float m_last_time;      // 记录上一次循环的时间刻度
};