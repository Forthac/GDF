#include "shader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

std::string loadShaderSource(const std::string& shaderFile) {
    std::ifstream file(shaderFile);
    if (!file) {
        std::cerr << "Error opening file: " << shaderFile << std::endl;
        return "";
    }

    std::string shaderSource((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    std::cout << "Loaded shader source from: " << shaderFile << std::endl;
    std::cout << shaderSource << std::endl;

    return shaderSource;
}
GLuint compileShader(GLenum shaderType, const std::string& shaderSource) {
    GLuint shader = glCreateShader(shaderType);
    const char* source = shaderSource.c_str();
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLint maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
        std::string errorMsg(errorLog.begin(), errorLog.end());
        std::cerr << "Shader compilation failed: " << errorMsg << std::endl;
        glDeleteShader(shader);
        return 0;
    }

    std::cout << "Shader compiled successfully: " << shader << std::endl;
    return shader;
}
GLuint createShaderProgram(const std::string& vertexShaderFile, const std::string& fragmentShaderFile) {
    std::string vertexShaderSource = loadShaderSource(vertexShaderFile);
    std::string fragmentShaderSource = loadShaderSource(fragmentShaderFile);

    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        GLint maxLength = 0;
        glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<GLchar> errorLog(maxLength);
        glGetProgramInfoLog(shaderProgram, maxLength, &maxLength, &errorLog[0]);
        std::string errorMsg(errorLog.begin(), errorLog.end());
        throw std::runtime_error("Shader program linking failed: " + errorMsg);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}