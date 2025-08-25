#version 460 core

in vec4 fragColor;
in vec3 fragNormal;
in vec3 fragPos;

out vec4 FragColor;

void main() {
    FragColor = fragColor;
}

