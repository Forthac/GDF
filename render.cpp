#include "render.h"

void updateUniforms(GLuint shaderProgram, int width, int height, float scaleX, float scaleY, float panX, float panY) {
    GLint textureSizeLocation = glGetUniformLocation(shaderProgram, "textureSize");
    glUniform2f(textureSizeLocation, (float)width, (float)height);

    GLint scaleLocation = glGetUniformLocation(shaderProgram, "scale");
    glUniform2f(scaleLocation, scaleX, scaleY);

    GLint panLocation = glGetUniformLocation(shaderProgram, "pan");
    glUniform2f(panLocation, panX, panY);

    //std::cout << "ScaleX: " << scaleX << ", ScaleY: " << scaleY << ", PanX: " << panX << ", PanY: " << panY << std::endl;
}
void render(GLuint framebuffer, GLuint shaderProgram, GLuint texture, GLuint VAO, int width, int height, float scaleX, float scaleY, float panX, float panY, float* clearColor) {
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glViewport(0, 0, width, height);

    glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    updateUniforms(shaderProgram, width, height, scaleX, scaleY, panX, panY);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void renderToTexture(GLFWwindow* window, GLuint framebuffer, GLuint shaderProgram, GLuint texture, GLuint VAO, int width, int height) {
    float clearColor[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    CallbackData* callbackData = static_cast<CallbackData*>(glfwGetWindowUserPointer(window));
    float panX = callbackData->panX;
    float panY = callbackData->panY;
    render(framebuffer, shaderProgram, texture, VAO, width, height, 1.0f, 1.0f, panX, panY, clearColor);
}
void renderToScreen(GLFWwindow* window, GLuint shaderProgram, GLuint texture, GLuint VAO, int width, int height) {
    float clearColor[4] = {0.2f, 0.3f, 0.3f, 1.0f};
    CallbackData* callbackData = static_cast<CallbackData*>(glfwGetWindowUserPointer(window));
    float scaleX = callbackData->scaleX;
    float scaleY = callbackData->scaleY;
    float panX = callbackData->panX;
    float panY = callbackData->panY;

    // Use the previous texture when the simulation is paused
    if (callbackData->isPaused && callbackData->prevTexture != -1) {
        texture = callbackData->textures[callbackData->prevTexture];
    }

    render(0, shaderProgram, texture, VAO, width, height, scaleX, scaleY, panX, panY, clearColor);
}