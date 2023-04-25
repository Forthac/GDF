#pragma once
#include <GL/glew.h>

GLuint loadTextureFromFile(const char* filename, int& width, int& height);
GLuint createSimulationDoubleBuffer(const char *filename, GLuint textures[2], GLuint framebuffers[2], int &width, int &height);