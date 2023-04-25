#include "render.h"
#include "input.h"

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        if (key == GLFW_KEY_ESCAPE) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        } else if (key == GLFW_KEY_SPACE) {
            // Toggle the pause state
            CallbackData* callbackData = static_cast<CallbackData*>(glfwGetWindowUserPointer(window));
            callbackData->isPaused = !callbackData->isPaused;
        }
        // Add more key handling logic here
    }
}

void setupInputCallbacks(GLFWwindow* window) {
    glfwSetKeyCallback(window, keyCallback);
    // Register other input callbacks here, e.g., mouse button, cursor position, etc.
}