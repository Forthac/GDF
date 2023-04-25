#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
GLFWwindow* initializeWindow(int width, int height);