#include <iostream>
#define SDL_MAIN_HANDLED
#include "SDL.h"
#include "glad/glad.h"
#include "shaderload.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "painter.hpp"
#include "decode/soft_decode_thd.h"

constexpr int OPENGL_MAJOR_VERSION = 3;
constexpr int OPENGL_MINOR_VERSION = 3;

int main() {
    int dialog_width = 1918;
    int dialog_height = 1078;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error:" << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, OPENGL_MAJOR_VERSION);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, OPENGL_MINOR_VERSION);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    int window_flag=  SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE;
    SDL_Window * window = SDL_CreateWindow("yuv_texture", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, dialog_width, dialog_height, window_flag);

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

    std::cout << "#############Main thread id: " << std::this_thread::get_id() << std::endl;

    CPainter oPainter(window, context);
    SDL_GL_MakeCurrent(window, nullptr);

    oPainter.RegisterMakeCurrentFunc([](void* window, void* context){
        if (window == nullptr || context == nullptr) {
            return;
        }
        std::cout << "#############other thread id: " << std::this_thread::get_id() << std::endl;
        SDL_GL_MakeCurrent(static_cast<SDL_Window*>(window), static_cast<SDL_GLContext>(context));
    });

    oPainter.RegisterSwapWindowFunc([](void *window){
        if (window == nullptr) {
            return;
        }
        SDL_GL_SwapWindow(static_cast<SDL_Window*>(window));
    });

    CSoftDecodeThd decode_thd;
    decode_thd.StartThd("C:\\Users\\MMK\\Desktop\\QQ_video.mp4", &oPainter);

    bool first_show = false;
    bool quit = false;
    while (!quit) {
        SDL_Event event;
        if (SDL_PollEvent(&event)) {
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
//                    SDL_WindowEvent &window_event = event.window;
//                    if (window_event.event == SDL_WINDOWEVENT_SHOWN) {
//                        if (!first_show) {
//                            first_show = true;
//                            oPainter.Viewport(0, 0, 800, 600);
//                        }
//                    }else if (window_event.event == SDL_WINDOWEVENT_RESIZED) {
//                        oPainter.Viewport(0, 0, window_event.data1, window_event.data2);
//                    }
                }
                    break;
                default:
                    break;
            }
        }
    }

    return 0;
}
