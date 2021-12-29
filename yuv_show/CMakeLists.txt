cmake_minimum_required(VERSION 3.19)
project(yuv_show)

set(CMAKE_CXX_STANDARD 17)

include_directories($ENV{SDL_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/glad/include)

add_executable(yuv_show main.cpp glad/src/glad.c shaderload.hpp shader/fragment_shader.h shader/vertex_shader.h stb_image.h)

target_link_directories(yuv_show PRIVATE $ENV{SDL_LIB_DIR})
target_link_libraries(yuv_show SDL2.lib)