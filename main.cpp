#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include "texture.h"
#include "quad.h"

void updateUniforms(GLuint shaderProgram, int width, int height) {
    GLint textureSizeLocation = glGetUniformLocation(shaderProgram, "textureSize");
    glUniform2f(textureSizeLocation, (float)width, (float)height);
}

void renderToTexture(GLuint framebuffer, GLuint shaderProgram, GLuint texture, GLuint VAO, int width, int height) {
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glViewport(0, 0, width, height);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    updateUniforms(shaderProgram, width, height);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void renderToScreen(GLuint shaderProgram, GLuint texture, GLuint VAO, int width, int height) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, width, height);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    updateUniforms(shaderProgram, width, height);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
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
        renderToTexture(framebuffers[1 - currentTexture], shaderProgram, textures[currentTexture], VAO, width, height);
        currentTexture = 1 - currentTexture;
        renderToScreen(shaderProgram, textures[currentTexture], VAO, 800, 600);
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
    GLFWwindow* window = initializeWindow();
    if (!window) {
        return -1;
    }

    GLuint shaderProgram = createShaderProgram("vertex_shader.vert", "game_of_life.frag");
    GLuint VBO, EBO, VAO;
    VAO = createQuadVAO(VBO, EBO);

    int width, height;
    GLuint textures[2];
    GLuint framebuffers[2];
    GLuint initialState;
    initialState = createSimulationDoubleBuffer("initial_state.bmp", textures, framebuffers, width, height);

    runMainLoop(window, shaderProgram, VAO, textures, framebuffers, width, height);

    cleanup(VAO, VBO, EBO, shaderProgram, textures, framebuffers);
    glfwTerminate();

    return 0;
}