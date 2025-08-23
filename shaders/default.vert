#version 460 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec2 aUV;
layout(location = 4) in vec2 aUV2;

out vec2 fragUV;

void main() {
    fragUV = aUV;
    gl_Position = vec4(aPos, 1.0);
}
