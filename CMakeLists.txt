cmake_minimum_required(VERSION 3.30)
project(tinyRenderer)

set(CMAKE_CXX_STANDARD 20)

include_directories(include/GLFW)
include_directories(include)

link_directories(dependencies/lib-mingw-w64)
find_package(OpenGL REQUIRED)

add_executable(tinyRenderer main.cpp
        include/vec2.h
        include/vec3.h
        include/color.h
        include/math_helper.h
        include/renderer.h
        include/model.h
        include/triangle.h
        triangle.cpp
        include/camera.h
)

target_link_libraries(tinyRenderer glfw3 opengl32)