#version 330 core

uniform sampler2D inputTexture;
uniform vec2 textureSize;
uniform float deltaTime;
uniform float time;

in vec2 fragTexCoord;

out vec4 fragColor;

int getNeighborState(ivec2 coord) {
    ivec2 wrappedCoord = clamp(coord, ivec2(0), ivec2(textureSize) - 1);
    return int(texelFetch(inputTexture, wrappedCoord, 0).r + 0.5);
}

void main() {
    ivec2 coord = ivec2(fragTexCoord * textureSize);
    int currentState = getNeighborState(coord);

    int liveNeighbors = 0;
    for (int y = -1; y <= 1; ++y) {
        for (int x = -1; x <= 1; ++x) {
            if (x != 0 || y != 0) {
                liveNeighbors += getNeighborState(coord + ivec2(x, y));
            }
        }
    }

    int nextState = currentState;
    if (currentState == 1) {
        if (liveNeighbors < 2 || liveNeighbors > 3) {
            nextState = 0;
        }
    } else {
        if (liveNeighbors == 3) {
            nextState = 1;
        }
    }

    fragColor = vec4(vec3(float(nextState)), 1.0);
}