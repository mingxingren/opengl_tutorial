#include "app.h"
#include "common/macrodef.h"
#include "shader_load.hpp"
#include "texture2d.h"

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

    auto shader_ptr = CShader::load_shader_width_path("shader/cube.vs.glsl", "shader/cube.fs.glsl", nullptr);
    float cube_vertexs[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertexs), (void*)cube_vertexs, GL_STATIC_DRAW);
    
    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    Texture2D texture(0, "res/cube_texture.jpg", shader_ptr);

    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window)) {
        this->_process_input(window);
        float current_time = glfwGetTime();
        float delta_time = current_time - m_last_time;

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
         
        shader_ptr->set_matrix("view", this->m_camera.get_view());

        // 创建 projection
        // @param 视角
        // @param 宽高比
        // @param 近处
        // @param 远处
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)this->m_dialog_width / (float)this->m_dialog_height, 0.1f, 100.0f);
        shader_ptr->set_matrix("projection", projection);

        // 创建 model 
        glm::mat4 model = glm::mat4(1.0f);
        shader_ptr->set_matrix("model", model);

        texture.bind();

        glDrawArrays(GL_TRIANGLES, 0, sizeof(cube_vertexs) / 5);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void App::framebuffer_size_callback(GLFWwindow* _window, int _width, int _height) {

}

void App::mouse_callback(GLFWwindow* _window, double _xpos, double _ypos) {

}

void App::scroll_callback(GLFWwindow* _window, double _xoffset, double _yoffset) {

}

void App::_process_input(GLFWwindow* window) {
    float cameraSpeed = 0.005f;
    glm::vec3 cameraFont = glm::vec3(0.0, 0.0, -1.0);
    std::cout << "###################### _process_input  GLFW_KEY_W state:" << glfwGetKey(window, GLFW_KEY_W)  << std::endl;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        this->m_camera.move(cameraSpeed * cameraFont);
        std::cout << "###################### www" << std::endl;
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        this->m_camera.move(-cameraSpeed * cameraFont);
    }
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        this->m_camera.right(cameraFont, cameraSpeed);
    }
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        this->m_camera.right(cameraFont, -cameraSpeed);
    }
}