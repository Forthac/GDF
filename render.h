#pragma once
#include <vector>
#include <iostream>
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
    float panX = 0.0f;
    float panY = 0.0f;
    bool isPaused;
    int prevTexture;
    bool advanceOneFrame;
};


void updateUniforms(GLuint shaderProgram, int width, int height, float scaleX, float scaleY);
void render(GLuint framebuffer, GLuint shaderProgram, GLuint texture, GLuint VAO, int width, int height, float scaleX, float scaleY, float panX, float panY, float clearColor[4]);
void renderToTexture(GLFWwindow* window, GLuint framebuffer, GLuint shaderProgram, GLuint texture, GLuint VAO, int width, int height);
void renderToScreen(GLFWwindow* window, GLuint shaderProgram, GLuint texture, GLuint VAO, int width, int height);