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
        } else {
            float panSpeed = 0.01f;
            switch (key) {
                case GLFW_KEY_UP:
                    callbackData->panY -= panSpeed;
                    break;
                case GLFW_KEY_DOWN:
                    callbackData->panY += panSpeed;
                    break;
                case GLFW_KEY_LEFT:
                    callbackData->panX += panSpeed;
                    break;
                case GLFW_KEY_RIGHT:
                    callbackData->panX -= panSpeed;
                    break;
            }
            std::cout << "PanX: " << callbackData->panX << ", PanY: " << callbackData->panY << std::endl;
        }
        // Add more key handling logic here
    }
}
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
    CallbackData* callbackData = static_cast<CallbackData*>(glfwGetWindowUserPointer(window));
    callbackData->scaleX *= (1.0f + 0.1f * yOffset);
    callbackData->scaleY *= (1.0f + 0.1f * yOffset);
}
void setupInputCallbacks(GLFWwindow* window) {
    glfwSetKeyCallback(window, keyCallback);
    glfwSetScrollCallback(window, scrollCallback);
    // Register other input callbacks here, e.g., mouse button, cursor position, etc.
}