#version 460 core

in vec2 fragUV;
out vec4 FragColor;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gMRAE;
uniform sampler2D gAlbedo;

void main() {
    vec3 albedo = texture(gAlbedo, fragUV).rgb;
    FragColor = vec4(albedo, 1.0);
}

