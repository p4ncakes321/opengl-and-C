#ifndef TRANSFORM_COMP_H
#define TRANSFORM_COMP_H

#include <cglm/types.h>
#include <stddef.h>

typedef struct TransformComponent {
    size_t instanceCount;
    mat4* modelMatrices;
    mat4* worldMatrices;
    vec3* positions;
    vec3* rotations;
    vec3* scales;
} TransformComponent;

TransformComponent* TransformComponentCreatev(mat4 modelMatrix);
TransformComponent* TransformComponentCreate(vec3 position, vec3 rotation, vec3 scale);
TransformComponent* TransformComponentCreateMultiple(size_t instanceCount, vec3* positions, vec3* rotations, vec3* scales);

void TransformComponentSetModelMatrix(TransformComponent* component, mat4 modelMatrix);
void TransformComponentSetPosition(TransformComponent* component, vec3 position);
void TransformComponentSetRotation(TransformComponent* component, vec3 rotation);
void TransformComponentSetScale(TransformComponent* component, vec3 scale);

void TransformComponentSetPositionAt(TransformComponent* component, size_t index, vec3 position);
void TransformComponentSetRotationAt(TransformComponent* component, size_t index, vec3 rotation);
void TransformComponentSetScaleAt(TransformComponent* component, size_t index, vec3 scale);

void TransformComponentGetAverageMatrix(TransformComponent* component, mat4 out);
void TransformComponentDestroy(TransformComponent* component);

#endif

