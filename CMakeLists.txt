cmake_minimum_required(VERSION 3.30)
project(tinyRenderer)

set(CMAKE_CXX_STANDARD 20)

add_executable(tinyRenderer main.cpp
        include/vec2.h
        include/vec3.h
        include/color.h
        include/math_helper.h
        include/draw.h
)
