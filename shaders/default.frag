#version 460 core

in vec4 fragColor;
in vec3 fragNormal;
in vec3 fragPos;
in vec2 fragUV;

out vec4 FragColor;
uniform sampler2D albedo;

void main() {
    vec3 albedoColor = texture(albedo, fragUV).rgb;
    vec3 finalColor = mix(albedoColor, fragColor.rgb, 0.5);

    FragColor = vec4(finalColor, 1.0);
}

