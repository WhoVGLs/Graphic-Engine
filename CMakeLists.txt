cmake_minimum_required(VERSION 3.20)

project(GraphicEngine VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(WIN32 AND EXISTS "C:/msys64/ucrt64")
    list(APPEND CMAKE_PREFIX_PATH "C:/msys64/ucrt64")
endif()

find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW CONFIG REQUIRED)

add_executable(GraphicEngine
    src/main.cpp
    src/shader.cpp
    src/VAO.cpp
    src/VBO.cpp
    src/EBO.cpp
)

target_include_directories(GraphicEngine PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(GraphicEngine PRIVATE
    OpenGL::GL
    glfw
    GLEW::GLEW
)

if(MSVC)
    target_compile_options(GraphicEngine PRIVATE /W4)
else()
    target_compile_options(GraphicEngine PRIVATE -Wall -Wextra -Wpedantic)
endif()
