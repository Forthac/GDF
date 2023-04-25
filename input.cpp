#include "render.h"
#include "input.h"

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    CallbackData* callbackData = static_cast<CallbackData*>(glfwGetWindowUserPointer(window));
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        if (key == GLFW_KEY_ESCAPE) {
            // Close the window if the escape key is pressed
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        } else if (key == GLFW_KEY_SPACE) {
            // Toggle the pause state
            callbackData->isPaused = !callbackData->isPaused;
        } else if (key == GLFW_KEY_ENTER && callbackData->isPaused) {
            // Advance one frame if paused
            callbackData->advanceOneFrame = true;
        }
        // Add more key handling logic here
    }
}

void setupInputCallbacks(GLFWwindow* window) {
    glfwSetKeyCallback(window, keyCallback);
    // Register other input callbacks here, e.g., mouse button, cursor position, etc.
}