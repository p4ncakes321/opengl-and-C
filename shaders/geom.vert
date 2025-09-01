#version 460 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec2 aUV;
layout(location = 4) in vec2 aUV2;

out vec3 fragPos;
out vec3 fragNormal;
out vec4 fragColor;
out vec2 fragUV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout(std430, binding = 0) buffer Matrices {
    mat4 instanceModel[];
};
uniform bool useInstances;

void main() {
    mat4 m = model;
    if (useInstances) {
        m = instanceModel[gl_InstanceID];
    }

    vec4 worldPos = m * vec4(aPos, 1.0);
    fragPos = worldPos.xyz;
    fragNormal = mat3(transpose(inverse(m))) * aNormal;
    fragColor = aColor;
    fragUV = aUV;

    gl_Position = projection * view * worldPos;
}

