#ifndef STATICMESH_H
#define STATICMESH_H

#include "mesh.h"

typedef struct StaticMesh {
    Mesh base;
    Vertex* vertices;
    GLuint* indices;  
} StaticMesh;

StaticMesh* StaticMeshCreate(Vertex* vertices, size_t vertexCount, GLuint* indices, size_t indexCount);

#endif
