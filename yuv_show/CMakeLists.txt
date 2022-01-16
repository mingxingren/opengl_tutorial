cmake_minimum_required(VERSION 3.19)
project(yuv_show)

set(CMAKE_CXX_STANDARD 17)

set(FFMPEG_LIB_PATH D:/third_lib/ffmpeg/4.4/lib)
set(FFMPEG_INCLUDE_PATH D:/third_lib/ffmpeg/4.4/include)
set(FFMPEG_LIBS avcodec.lib avdevice.lib avfilter.lib
        avformat.lib avutil.lib postproc.lib swresample.lib swscale.lib)

include_directories(${FFMPEG_INCLUDE_PATH})
include_directories($ENV{SDL_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/glad/include)

add_executable(yuv_show main.cpp glad/src/glad.c shaderload.hpp shader/fragment_shader.h shader/vertex_shader.h stb_image.h
        decode/soft_decode_thd.h decode/soft_decode_thd.cpp painter.hpp)

target_link_directories(yuv_show PRIVATE $ENV{SDL_LIB_DIR})
target_link_libraries(yuv_show SDL2.lib)

target_link_directories(yuv_show PRIVATE ${FFMPEG_LIB_PATH})
target_link_libraries(yuv_show ${FFMPEG_LIBS})