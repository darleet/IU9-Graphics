cmake_minimum_required(VERSION 3.26)
project(lab2)

set(CMAKE_CXX_STANDARD 20)

add_executable(lab2 main.cpp
        main.h)

target_link_libraries(
        ${PROJECT_NAME} glfw GL dl
)
