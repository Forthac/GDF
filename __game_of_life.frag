#version 330 core

uniform sampler2D inputTexture;

in vec2 fragTexCoord;

out vec4 fragColor;

void main() {
    vec3 color = texture(inputTexture, fragTexCoord).rgb;
    fragColor = vec4(color, 1.0);
}