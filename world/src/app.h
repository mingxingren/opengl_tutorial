#pragma once

#include <memory>
#include <mutex>
#include "../deps/glad/glad.h"
#include "GLFW/glfw3.h"
#include "camera.h"

class App {
public:
    explicit App();
    App(const App&) = delete;
    ~App();
    void run();

private:
    void _process_input(GLFWwindow* window);

private:
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    static void keyborad_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

private:
    static std::once_flag g_init_flag;
    static std::unique_ptr<App> g_app;

private:
    GLFWwindow* m_window;    
    int m_dialog_width = 800;
    int m_dialog_height = 600;

    float m_last_time;      // 记录上一次循环的时间刻度

    Camera m_camera;        // 摄像机
};