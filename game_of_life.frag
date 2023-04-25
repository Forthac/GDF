#version 330 core

uniform sampler2D inputTexture;
uniform vec2 textureSize;

in vec2 fragTexCoord;

out vec4 fragColor;

int getNeighborState(ivec2 coord) {
    ivec2 wrappedCoord = clamp(coord, ivec2(0), ivec2(textureSize) - 1);
    return int(texelFetch(inputTexture, wrappedCoord, 0).r > 0.5);
}

void main() {
    ivec2 texCoord = ivec2(fragTexCoord * textureSize);
    int currentState = getNeighborState(texCoord);
    int numNeighbors = 0;

    for (int y = -1; y <= 1; ++y) {
        for (int x = -1; x <= 1; ++x) {
            if (x == 0 && y == 0) continue;
            numNeighbors += getNeighborState(texCoord + ivec2(x, y));
        }
    }

    int nextState = currentState;
    if (currentState == 1 && (numNeighbors < 2 || numNeighbors > 3)) {
        nextState = 0;
    } else if (currentState == 0 && numNeighbors == 3) {
        nextState = 1;
    }

    fragColor = nextState == 1 ? vec4(1.0, 1.0, 1.0, 1.0) : vec4(0.0, 0.0, 0.0, 1.0);
}