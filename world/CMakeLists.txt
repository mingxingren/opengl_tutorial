cmake_minimum_required(VERSION 3.0)
# 我的诗歌
project(my_poetry)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

# # 使用 Cpp assert
# add_definitions(NDEBUG)

# 添加 opengl 3.3 glad 加文件 
include_directories(deps/glad)
FILE(GLOB SRC_GLAD_FILES deps/glad/*.c)

# 列出所有的.c 文件 .cpp文件
FILE(GLOB SRC_FILES src/*.c*)
# 列出所有的.hpp 文件
FILE(GLOB HPP_FILES src/*.hpp)


message(${SRC_FILES})
add_executable(my_poetry ${SRC_FILES} ${HPP_FILES} ${SRC_GLAD_FILES})
target_include_directories(my_poetry PRIVATE D:\\third_lib\\openGL\\glfw-3.3.5.bin.WIN64\\include)
target_link_directories(my_poetry PRIVATE D:\\third_lib\\openGL\\glfw-3.3.5.bin.WIN64\\lib-mingw-w64)
target_link_libraries(my_poetry -lglfw3)

