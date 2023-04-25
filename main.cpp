#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include "texture.h"
#include "quad.h"


int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(800, 600, "GDF", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
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

    int currentTexture = 0;

    // Set the textureSize uniform
    GLint textureSizeLocation = glGetUniformLocation(shaderProgram, "textureSize");
    glUniform2f(textureSizeLocation, (float)width, (float)height);


    while (!glfwWindowShouldClose(window)) {
        // Render to the next state texture
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffers[1 - currentTexture]);
        glViewport(0, 0, width, height);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        GLint textureSizeLocation = glGetUniformLocation(shaderProgram, "textureSize");
        glUniform2f(textureSizeLocation, (float)width, (float)height);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[currentTexture]);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Swap current and next state textures
        currentTexture = 1 - currentTexture;

        // Render the current state texture to the default framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, 800, 600);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glUniform2f(textureSizeLocation, (float)width, (float)height);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[currentTexture]);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
    glDeleteTextures(2, textures);
    glDeleteFramebuffers(2, framebuffers);

    glfwTerminate();
    return 0;
}