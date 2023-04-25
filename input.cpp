#include "input.h"

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        if (key == GLFW_KEY_ESCAPE) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
        // Add more key handling logic here
    }
}

void setupInputCallbacks(GLFWwindow* window) {
    glfwSetKeyCallback(window, keyCallback);
    // Register other input callbacks here, e.g., mouse button, cursor position, etc.
}