#pragma once
#include <GL/glew.h>
typedef GLuint (*TextureGeneratorFunc)(int, int, float);

GLuint createRandomInitialStateTexture(int width, int height, float aliveProbability);
GLuint loadTextureFromFile(const char* filename, int& width, int& height);
GLuint createSimulationDoubleBuffer(TextureGeneratorFunc textureGenerator, float aliveProbability, GLuint textures[2], GLuint framebuffers[2], int &width, int &height);
GLuint createSimulationDoubleBuffer(const char *filename, GLuint textures[2], GLuint framebuffers[2], int &width, int &height);