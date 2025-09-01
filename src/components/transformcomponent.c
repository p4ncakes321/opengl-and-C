#include "components/transformcomponent.h"
#include <cglm/cglm.h>
#include <stdlib.h>

static void TransformComponentCalculateMatrixAt(TransformComponent* component, size_t index) {
    if (index >= component->instanceCount) return;

    glm_mat4_identity(component->modelMatrices[index]);
    glm_translate(component->modelMatrices[index], component->positions[index]);
    glm_rotate_x(component->modelMatrices[index], component->rotations[index][0], component->modelMatrices[index]);
    glm_rotate_y(component->modelMatrices[index], component->rotations[index][1], component->modelMatrices[index]);
    glm_rotate_z(component->modelMatrices[index], component->rotations[index][2], component->modelMatrices[index]);
    glm_scale(component->modelMatrices[index], component->scales[index]);
}

TransformComponent* TransformComponentCreatev(mat4 modelMatrix) {
    TransformComponent* component = malloc(sizeof(TransformComponent));
    if (!component) return NULL;

    component->instanceCount = 1;

    component->modelMatrices = malloc(sizeof(mat4));
    glm_mat4_copy(modelMatrix, component->modelMatrices[0]);

    component->worldMatrices = malloc(sizeof(mat4));
    glm_mat4_identity(component->worldMatrices[0]);

    component->positions = malloc(sizeof(vec3));
    glm_vec3_zero(component->positions[0]);

    component->rotations = malloc(sizeof(vec3));
    glm_vec3_zero(component->rotations[0]);

    component->scales = malloc(sizeof(vec3));
    glm_vec3_one(component->scales[0]);

    return component;
}

TransformComponent* TransformComponentCreate(vec3 position, vec3 rotation, vec3 scale) {
    TransformComponent* component = malloc(sizeof(TransformComponent));
    if (!component) return NULL;

    component->instanceCount = 1;
    component->worldMatrices = malloc(sizeof(mat4));
    component->modelMatrices = malloc(sizeof(mat4));
    component->positions = malloc(sizeof(vec3));
    component->rotations = malloc(sizeof(vec3));
    component->scales = malloc(sizeof(vec3));

    glm_mat4_identity(component->worldMatrices[0]);
    glm_vec3_copy(position, component->positions[0]);
    glm_vec3_copy(rotation, component->rotations[0]);
    glm_vec3_copy(scale, component->scales[0]);

    TransformComponentCalculateMatrixAt(component, 0);
    return component;
}

TransformComponent* TransformComponentCreateMultiple(size_t instanceCount, vec3* positions, vec3* rotations, vec3* scales) {
    TransformComponent* component = malloc(sizeof(TransformComponent));
    if (!component) return NULL;

    component->instanceCount = instanceCount;

    component->modelMatrices = malloc(sizeof(mat4) * instanceCount);
    component->positions     = malloc(sizeof(vec3) * instanceCount);
    component->rotations     = malloc(sizeof(vec3) * instanceCount);
    component->scales        = malloc(sizeof(vec3) * instanceCount);

    component->worldMatrices = malloc(sizeof(mat4) * component->instanceCount);
    for (size_t i = 0; i < component->instanceCount; i++)
        glm_mat4_identity(component->worldMatrices[i]);


    for (size_t i = 0; i < instanceCount; i++) {
        glm_vec3_copy(positions[i], component->positions[i]);
        glm_vec3_copy(rotations[i], component->rotations[i]);
        glm_vec3_copy(scales[i], component->scales[i]);
        TransformComponentCalculateMatrixAt(component, i);
    }

    return component;
}

void TransformComponentGetAverageMatrix(TransformComponent* component, mat4 out) {
    if (!component || component->instanceCount == 0) {
        glm_mat4_identity(out);
        return;
    }

    vec3 avgPos = {0.0f, 0.0f, 0.0f};
    vec3 avgRot = {0.0f, 0.0f, 0.0f};

    for (size_t i = 0; i < component->instanceCount; i++) {
        glm_vec3_add(avgPos, component->positions[i], avgPos);
        glm_vec3_add(avgRot, component->rotations[i], avgRot);
    }

    float invCount = 1.0f / (float)component->instanceCount;
    glm_vec3_scale(avgPos, invCount, avgPos);
    glm_vec3_scale(avgRot, invCount, avgRot);

    mat4 trans, rot;
    glm_translate_make(trans, avgPos);
    glm_euler_xyz(avgRot, rot);

    glm_mat4_mul(trans, rot, out);
}

void TransformComponentSetModelMatrix(TransformComponent* component, mat4 modelMatrix) {
    if (component->instanceCount == 0) return;
    glm_mat4_copy(modelMatrix, component->modelMatrices[0]);
}

void TransformComponentSetPosition(TransformComponent* component, vec3 position) {
    if (component->instanceCount == 0) return;
    glm_vec3_copy(position, component->positions[0]);
    TransformComponentCalculateMatrixAt(component, 0);
}

void TransformComponentSetRotation(TransformComponent* component, vec3 rotation) {
    if (component->instanceCount == 0) return;
    glm_vec3_copy(rotation, component->rotations[0]);
    TransformComponentCalculateMatrixAt(component, 0);
}

void TransformComponentSetScale(TransformComponent* component, vec3 scale) {
    if (component->instanceCount == 0) return;
    glm_vec3_copy(scale, component->scales[0]);
    TransformComponentCalculateMatrixAt(component, 0);
}

void TransformComponentSetPositionAt(TransformComponent* component, size_t index, vec3 position) {
    if (index >= component->instanceCount) return;
    glm_vec3_copy(position, component->positions[index]);
    TransformComponentCalculateMatrixAt(component, index);
}

void TransformComponentSetRotationAt(TransformComponent* component, size_t index, vec3 rotation) {
    if (index >= component->instanceCount) return;
    glm_vec3_copy(rotation, component->rotations[index]);
    TransformComponentCalculateMatrixAt(component, index);
}

void TransformComponentSetScaleAt(TransformComponent* component, size_t index, vec3 scale) {
    if (index >= component->instanceCount) return;
    glm_vec3_copy(scale, component->scales[index]);
    TransformComponentCalculateMatrixAt(component, index);
}
