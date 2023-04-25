#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include "texture.h"
#include "quad.h"
#include "render.h"
#include "window.h"
#include "input.h"

void runMainLoop(GLFWwindow* window, GLuint shaderProgram, GLuint VAO, GLuint textures[], GLuint framebuffers[], int width, int height) {
    int currentTexture = 0;

    while (!glfwWindowShouldClose(window)) {
        CallbackData* callbackData = static_cast<CallbackData*>(glfwGetWindowUserPointer(window));
        int width = callbackData->windowWidth;
        int height = callbackData->windowHeight;

        if (!callbackData->isPaused || callbackData->advanceOneFrame) {
            renderToTexture(framebuffers[1 - currentTexture], shaderProgram, textures[currentTexture], VAO, width, height);
            currentTexture = 1 - currentTexture;
            if (callbackData->advanceOneFrame) {
                callbackData->advanceOneFrame = false;
            }
        }

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
    callbackData.isPaused = false;
    callbackData.prevTexture = -1;

    GLFWwindow* window = initializeWindow(width, height);
    if (!window) {
        return -1;
    }

    // Set up input callbacks
    setupInputCallbacks(window);

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