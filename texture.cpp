#include <iostream>
#include <random>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "texture.h"

GLuint createEmptyTexture(int width, int height) {
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}
GLuint createRandomInitialStateTexture(int width, int height, float aliveProbability) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::bernoulli_distribution dist(aliveProbability);

    std::vector<unsigned char> data(width * height * 3);

    for (int i = 0; i < width * height; ++i) {
        unsigned char state = dist(gen) ? 255 : 0;
        data[i * 3] = state;
        data[i * 3 + 1] = state;
        data[i * 3 + 2] = state;
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}

GLuint loadTextureFromFile(const char* filename, int& width, int& height) {
    int channels;
    unsigned char *data = stbi_load(filename, &width, &height, &channels, STBI_rgb);

    if (!data) {
        std::cerr << "Failed to load image" << std::endl;
        return 0;
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);

    return texture;
}
GLuint createSimulationDoubleBuffer(const char *filename, GLuint textures[2], GLuint framebuffers[2], int &width, int &height) {
    glGenTextures(2, textures);
    glGenFramebuffers(2, framebuffers);

    // First texture and framebuffer
    textures[0] = loadTextureFromFile(filename, width, height);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffers[0]);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textures[0], 0);

    // Second texture and framebuffer
    textures[1] = createEmptyTexture(width, height);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffers[1]);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textures[1], 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    return textures[0];
}

GLuint createSimulationDoubleBuffer(TextureGeneratorFunc textureGenerator, float aliveProbability, GLuint textures[2], GLuint framebuffers[2], int &width, int &height) {
    glGenTextures(2, textures);
    glGenFramebuffers(2, framebuffers);

    // First texture and framebuffer
    textures[0] = textureGenerator(width, height, aliveProbability);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffers[0]);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textures[0], 0);

    // Second texture and framebuffer
    textures[1] = createEmptyTexture(width, height);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffers[1]);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textures[1], 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    return textures[0];
}