#ifndef MESH_COMP_H
#define MESH_COMP_H

#include "mesh.h"
#include <stddef.h>

typedef struct MeshComponent {
    Mesh* mesh;
    size_t instanceCount;
} MeshComponent;

MeshComponent* MeshComponentCreate(Mesh* mesh, size_t instanceCount);
void MeshComponentSetMesh(MeshComponent* component, Mesh* mesh);
void MeshComponentSetInstanceCount(MeshComponent* component, size_t instanceCount);

void MeshComponentDestroy(MeshComponent* component); // depending on how you make it work, either give the component or a void pointer (void pointer is easier and preffered).

#endif

