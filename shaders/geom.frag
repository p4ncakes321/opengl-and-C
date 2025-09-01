#version 460 core

in vec3 fragPos;
in vec3 fragNormal;
in vec4 fragColor;
in vec2 fragUV;

layout(location = 0) out vec3 gPosition;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec4 gAlbedo;
layout(location = 3) out vec4 gMRAE; 

uniform sampler2D albedoMap;
uniform float metallic;
uniform float roughness;
uniform float ao;
uniform float emission;

void main() {
    gPosition = fragPos;
    gNormal = normalize(fragNormal);
    gAlbedo.rgb = texture(albedoMap, fragUV).rgb;
    gAlbedo.a = 1.0;
    
    gMRAE.r = metallic;
    gMRAE.g = roughness;
    gMRAE.b = ao;
    gMRAE.a = emission;
}
