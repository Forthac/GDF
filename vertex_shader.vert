#version 330 core
layout (location = 0) in vec2 aPosition;
layout (location = 1) in vec2 aTexCoord;

uniform vec2 scale;

out vec2 fragTexCoord;

void main() {
    gl_Position = vec4(aPosition * scale, 0.0, 1.0);
    fragTexCoord = aTexCoord;
}
