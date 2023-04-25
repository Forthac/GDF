#include "render.h"

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