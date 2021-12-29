#include <iostream>
#define SDL_MAIN_HANDLED
#include "SDL.h"
#include "glad/glad.h"
#include "shaderload.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

constexpr int OPENGL_MAJOR_VERSION = 3;
constexpr int OPENGL_MINOR_VERSION = 3;

float space_vertices[] = {
        // 顶点坐标            // 纹理坐标
        1.0f, 1.0f, 0.0f,    1.0f, 1.0f, // 右上
        1.0f, -1.0f, 0.0f,   1.0f, 0.0f, // 右下
        -1.0f, -1.0f, 0.0f,  0.0f, 0.0f, // 左下
        -1.0f, 1.0f, 0.0f,   0.0f, 1.0f, // 左上
};

unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
};

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error:" << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, OPENGL_MAJOR_VERSION);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, OPENGL_MINOR_VERSION);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    int window_flag=  SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE;
    SDL_Window * window = SDL_CreateWindow("yuv_texture", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, window_flag);

    if (window == nullptr) {
        std::cerr << "There was an error creating the window: " << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }

    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (context == nullptr) {
        std::cerr << "There was an error creating OpenGL context: " << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }else{
        std::cout << "Creating OpenGL context created.........." << std::endl;
    }

    SDL_GL_MakeCurrent(window, context);

    if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) {
        std::cerr << "gladLoadGLLoader call fail" << std::endl;
    }else{
        std::cout << "gladLoadGLLoader call success" << std::endl;
    }

    CShaderload shader_load;

    unsigned int VAO, VBO, EBO;
    // 申请顶点缓冲队列
    glGenVertexArrays(1, &VAO);
    // 申请两块内存
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    {
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(space_vertices), space_vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // 将数据绑定到 location 0 变量
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        // 激活 location 0 变量
        glEnableVertexAttribArray(0);

        // 将数据绑定到 location 1 变量
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        // 激活 location 1 变量
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
    }

    unsigned int texture1;
    unsigned char* data = nullptr;
    int width, height, nrChannels;
    {
        glGenTextures(1, &texture1);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // 读取 texture 纹理
        data = stbi_load("wall.jpg", &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);
    }

    shader_load.Use();
    shader_load.SetInt("texture1", 0);

    bool first_show = false;
    bool quit = false;
    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_KEYDOWN:
                    quit = true;
                    break;
                case SDL_MOUSEMOTION:
                    break;
                case SDL_WINDOWEVENT: {
                    SDL_WindowEvent &window_event = event.window;
                    if (window_event.event == SDL_WINDOWEVENT_SHOWN) {
                        if (!first_show) {
                            first_show = true;
                            glViewport(0, 0, 800, 600);
                        }
                    }else if (window_event.event == SDL_WINDOWEVENT_RESIZED) {
                        glViewport(0, 0, window_event.data1, window_event.data2);
                    }
                }
                    break;
                default:
                    break;
            }
        }
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        shader_load.Use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

        SDL_GL_SwapWindow(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteTextures(1, &texture1);
    return 0;
}
