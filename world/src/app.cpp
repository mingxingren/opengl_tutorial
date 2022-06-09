#include "app.h"
#include "common/macrodef.h"
#include "shader_load.hpp"
#include "texture2d.h"
#include "cube_sprite.h"
#include "box_sprite.h"
#include "sphere_sprite.h"

std::once_flag App::g_init_flag;
std::unique_ptr<App> App::g_app = nullptr;

App::App() {
    std::call_once(App::g_init_flag, [](){
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    });
    g_app.reset(this);
}

App::~App() {

}

void App::run() {
    GLFWwindow* window = glfwCreateWindow(this->m_dialog_width, this->m_dialog_height, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        LOG("Failed to create GLFW window")
        glfwTerminate();
        return;
    }
    else {
        this->m_window = window;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    BoxSprite box("res/cube_texture.jpg");
    SphereSprite sphere = SphereSprite();

    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window)) {
        this->_process_input(window);
        float current_time = glfwGetTime();
        float delta_time = current_time - m_last_time;

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // 每次渲染清除背景颜色缓冲 和 深度缓冲
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // shader_ptr->use(); 
        // shader_ptr->set_matrix("view", this->m_camera.get_view());

        // 创建 projection
        // @param 视角
        // @param 宽高比
        // @param 近处
        // @param 远处
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)this->m_dialog_width / (float)this->m_dialog_height, 0.1f, 100.0f);

        // 创建 model 
        glm::mat4 model = glm::mat4(1.0f);
        box.draw(model, this->m_camera.get_view(), projection);

        glm::translate();
        sphere.draw(model, this->m_camera.get_view(), projection);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void App::framebuffer_size_callback(GLFWwindow* _window, int _width, int _height) {

}

void App::mouse_callback(GLFWwindow* _window, double _xpos, double _ypos) {
    float xpos = static_cast<float>(_xpos);
    float ypos = static_cast<float>(_ypos);

    if (g_app->m_first_mouse) {
        g_app->m_first_mouse = false;
        g_app->m_last_x = xpos;
        g_app->m_last_y = ypos;
    }

    float xoffset = xpos - g_app->m_last_x;
    float yoffset = g_app->m_last_y - ypos;
    g_app->m_last_x = xpos;
    g_app->m_last_y = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    g_app->m_yaw += xoffset;
    g_app->m_pitch += yoffset;

    if (g_app->m_pitch > 89.0f){
        g_app->m_pitch = 89.0f;
    }

    if (g_app->m_pitch < -89.0f) {
        g_app->m_pitch = -89.0f;
    }

    glm::vec3 front;
    front.x = cos(glm::radians(g_app->m_yaw)) * cos(glm::radians(g_app->m_pitch));
    front.y = sin(glm::radians(g_app->m_pitch));
    front.z = sin(glm::radians(g_app->m_yaw)) * cos(glm::radians(g_app->m_pitch));
    glm::vec3 front_normal = glm::normalize(front);

    g_app->m_camera.set_front(front_normal);
}

void App::scroll_callback(GLFWwindow* _window, double _xoffset, double _yoffset) {

}

void App::_process_input(GLFWwindow* window) {
    float cameraSpeed = 0.005f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        this->m_camera.move(cameraSpeed);
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        this->m_camera.move(-cameraSpeed);
    }
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        this->m_camera.right(cameraSpeed);
    }
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        this->m_camera.right(-cameraSpeed);
    }
}