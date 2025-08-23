#version 460 core

out vec4 fragColor;
in vec2 fragUV;

uniform sampler2D texture0;

void main() {
    fragColor = vec4(texture(texture0, fragUV).rgb,1.0f);
}
