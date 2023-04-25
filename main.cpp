#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include "texture.h"
#include "quad.h"
#include "render.h"

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

void runMainLoop(GLFWwindow* window, GLuint shaderProgram, GLuint VAO, GLuint textures[], GLuint framebuffers[], int width, int height) {
    int currentTexture = 0;

    while (!glfwWindowShouldClose(window)) {
        CallbackData* callbackData = static_cast<CallbackData*>(glfwGetWindowUserPointer(window));
        int width = callbackData->windowWidth;
        int height = callbackData->windowHeight;
        renderToTexture(framebuffers[1 - currentTexture], shaderProgram, textures[currentTexture], VAO, width, height);
        currentTexture = 1 - currentTexture;
        renderToScreen(window, shaderProgram, textures[currentTexture], VAO, width, height);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}


void cleanup(GLuint VAO, GLuint VBO, GLuint EBO, GLuint shaderProgram, GLuint textures[], GLuint framebuffers[]) {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
    glDeleteTextures(2, textures);
    glDeleteFramebuffers(2, framebuffers);
}

int main() {
    GLuint textures[2];
    GLuint framebuffers[2];
    GLuint initialState;
    int width = 1280;
    int height = 768;
    float aliveProbability = 0.1;
    CallbackData callbackData;
    callbackData.windowWidth = width;
    callbackData.windowHeight = height;
    callbackData.originalWidth = width;
    callbackData.originalHeight = height;
    callbackData.textures = textures;
    callbackData.framebuffers = framebuffers;
    callbackData.scaleX = 1.0f;
    callbackData.scaleY = 1.0f;

    GLFWwindow* window = initializeWindow(width, height);
    if (!window) {
        return -1;
    }

    GLuint shaderProgram = createShaderProgram("vertex_shader.vert", "game_of_life.frag");
    GLuint VBO, EBO, VAO;
    VAO = createQuadVAO(VBO, EBO);

    
    createSimulationDoubleBuffer(createRandomInitialStateTexture, aliveProbability, textures, framebuffers, width, height);

    glfwSetWindowUserPointer(window, &callbackData);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    runMainLoop(window, shaderProgram, VAO, textures, framebuffers, width, height);

    cleanup(VAO, VBO, EBO, shaderProgram, textures, framebuffers);
    glfwTerminate();

    return 0;
}