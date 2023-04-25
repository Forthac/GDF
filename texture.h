#pragma once
#include <GL/glew.h>
typedef GLuint (*TextureGeneratorFunc)(int, int, float);

void checkGLError(const char* message);
void resizeSimulationDoubleBuffer(GLuint textures[2], GLuint framebuffers[2], int newWidth, int newHeight);
GLuint createRandomInitialStateTexture(int width, int height, float aliveProbability);
GLuint loadTextureFromFile(const char* filename, int& width, int& height);
void resizeTextures(GLuint textures[2], GLuint framebuffers[2], int newWidth, int newHeight);
GLuint createSimulationDoubleBuffer(TextureGeneratorFunc textureGenerator, float aliveProbability, GLuint textures[2], GLuint framebuffers[2], int &width, int &height);
GLuint createSimulationDoubleBuffer(const char *filename, GLuint textures[2], GLuint framebuffers[2], int &width, int &height);