#include "components/meshcomponent.h"
#include <stdlib.h>

MeshComponent* MeshComponentCreate(Mesh* mesh, size_t instanceCount) {
    MeshComponent* component = (MeshComponent*)malloc(sizeof(MeshComponent));
    if (!component) return NULL;

    component->mesh = mesh;
    component->instanceCount = instanceCount;
    return component;
}

void MeshComponentSetMesh(MeshComponent* component, Mesh* mesh) {
    if (!component) return;
    component->mesh = mesh;
}

void MeshComponentSetInstanceCount(MeshComponent* component, size_t instanceCount) {
    if (!component) return;
    component->instanceCount = instanceCount;
}
