#pragma once
#include <vector>
#include "texture.h"
#include <GLFW/glfw3.h>

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

void updateUniforms(GLuint shaderProgram, int width, int height, float scaleX, float scaleY);
void render(GLuint framebuffer, GLuint shaderProgram, GLuint texture, GLuint VAO, int width, int height, float scaleX, float scaleY, float clearColor[4]);
void renderToTexture(GLuint framebuffer, GLuint shaderProgram, GLuint texture, GLuint VAO, int width, int height);
void renderToScreen(GLFWwindow* window, GLuint shaderProgram, GLuint texture, GLuint VAO, int width, int height);