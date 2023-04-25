#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include "texture.h"
#include "quad.h"

struct CallbackData {
    int windowWidth;
    int windowHeight;
    int originalWidth;
    int originalHeight;
    GLuint* textures;
    GLuint* framebuffers;
    float scaleX;
    float scaleY;
};

void resizeTextures(GLuint textures[2], GLuint framebuffers[2], int newWidth, int newHeight) {
    for (int i = 0; i < 2; ++i) {
        glBindTexture(GL_TEXTURE_2D, textures[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, newWidth, newHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

        glBindFramebuffer(GL_FRAMEBUFFER, framebuffers[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textures[i], 0);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);

    CallbackData* callbackData = static_cast<CallbackData*>(glfwGetWindowUserPointer(window));
    callbackData->windowWidth = width;
    callbackData->windowHeight = height;
    callbackData->scaleX = static_cast<float>(width) / static_cast<float>(callbackData->originalWidth);
    callbackData->scaleY = static_cast<float>(height) / static_cast<float>(callbackData->originalHeight);
}


void updateUniforms(GLuint shaderProgram, int width, int height, float scaleX, float scaleY) {
    GLint textureSizeLocation = glGetUniformLocation(shaderProgram, "textureSize");
    glUniform2f(textureSizeLocation, (float)width, (float)height);

    GLint scaleLocation = glGetUniformLocation(shaderProgram, "scale");
    glUniform2f(scaleLocation, scaleX, scaleY);
}


void render(GLuint framebuffer, GLuint shaderProgram, GLuint texture, GLuint VAO, int width, int height, float scaleX, float scaleY, float clearColor[4]) {
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glViewport(0, 0, width, height);

    glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    updateUniforms(shaderProgram, width, height, scaleX, scaleY);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}


void renderToTexture(GLuint framebuffer, GLuint shaderProgram, GLuint texture, GLuint VAO, int width, int height) {
    float clearColor[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    render(framebuffer, shaderProgram, texture, VAO, width, height, 1.0f, 1.0f, clearColor);
}

void renderToScreen(GLFWwindow* window, GLuint shaderProgram, GLuint texture, GLuint VAO, int width, int height) {
    float clearColor[4] = {0.2f, 0.3f, 0.3f, 1.0f};
    CallbackData* callbackData = static_cast<CallbackData*>(glfwGetWindowUserPointer(window));
    float scaleX = callbackData->scaleX;
    float scaleY = callbackData->scaleY;
    render(0, shaderProgram, texture, VAO, width, height, scaleX, scaleY, clearColor);
}

GLFWwindow* initializeWindow() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return nullptr;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "GDF", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
        return nullptr;
    }

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
    int width = 1024;
    int height = 1024;
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

    GLFWwindow* window = initializeWindow();
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