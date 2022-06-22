cmake_minimum_required(VERSION 3.0.0)
project(Materials VERSION 0.1.0)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

include_directories(deps)

file(GLOB_RECURSE DEPS_FILE_HPP deps/*.hpp)
file(GLOB_RECURSE DEPS_FILE_C deps/*.c)

file(GLOB_RECURSE SRC_FILES_CPP src/*.cpp)
file(GLOB_RECURSE SRC_FILES_CC src/*.cc)
file(GLOB_RECURSE SRC_FILES_C src/*.c)
file(GLOB_RECURSE SRC_FILES_HPP src/*.hpp)

add_executable(Lighting_Cast ${SRC_FILES_CPP} ${SRC_FILES_CC} \ 
                        ${SRC_FILES_C} ${SRC_FILES_HPP} ${DEPS_FILE_HPP} ${DEPS_FILE_C})

target_include_directories(Lighting_Cast PRIVATE D:\\third_lib\\openGL\\glfw-3.3.5.bin.WIN64\\include)
target_link_directories(Lighting_Cast PRIVATE D:\\third_lib\\openGL\\glfw-3.3.5.bin.WIN64\\lib-mingw-w64)
target_link_libraries(Lighting_Cast -lglfw3)

