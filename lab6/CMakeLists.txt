cmake_minimum_required(VERSION 3.27)
project(lab6)

set(CMAKE_CXX_STANDARD 20)

add_executable(lab6 main.cpp)

target_link_libraries(
        ${PROJECT_NAME} glfw GL dl
)

