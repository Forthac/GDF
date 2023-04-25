#include <iostream>
#include "render.h"
#include "window.h"

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);

    CallbackData* callbackData = static_cast<CallbackData*>(glfwGetWindowUserPointer(window));
    callbackData->windowWidth = width;
    callbackData->windowHeight = height;
    callbackData->scaleX = static_cast<float>(width) / static_cast<float>(callbackData->originalWidth);
    callbackData->scaleY = static_cast<float>(height) / static_cast<float>(callbackData->originalHeight);

    // Resize the textures and framebuffers
    resizeSimulationDoubleBuffer(callbackData->textures, callbackData->framebuffers, width, height);
}
GLFWwindow* initializeWindow(int width, int height) {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return nullptr;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    GLFWwindow* window = glfwCreateWindow(width, height, "GDF", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable VSync

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    return window;
}