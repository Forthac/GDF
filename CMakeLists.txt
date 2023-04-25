cmake_minimum_required(VERSION 3.0)
project(GDF)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)

set(GLEW_INCLUDE_DIR "C:/Users/forth/Documents/Development/C/GDF/glew/include")
set(GLEW_LIBRARY "C:/Users/forth/Documents/Development/C/GDF/glew/lib/Release/x64/glew32.lib")
set(GLFW_INCLUDE_DIR "C:/Users/forth/Documents/Development/C/GDF/glfw/include")
set(GLFW_LIBRARY "C:/Users/forth/Documents/Development/C/GDF/glfw/lib-vc2022/glfw3.lib")

add_executable(${PROJECT_NAME} main.cpp shader.cpp texture.cpp quad.cpp render.cpp window.cpp input.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE ${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIR} ${GLFW_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLEW_LIBRARY} ${GLFW_LIBRARY} glm::glm)