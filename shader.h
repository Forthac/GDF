#pragma once
#include <string>
#include <GL/glew.h>

std::string loadShaderSource(const std::string& shaderFile);
GLuint compileShader(GLenum shaderType, const std::string& shaderSource);
GLuint createShaderProgram(const std::string& vertexShaderFile, const std::string& fragmentShaderFile);