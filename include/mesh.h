#ifndef MESH_H
#define MESH_H

#include "glad/glad.h"
#include <cglm/cglm.h>

typedef struct Vertex {
    vec3 position;
    vec4 color;
    vec3 normal;
    vec2 uv;
    vec2 uv2;
} Vertex;

typedef struct Mesh Mesh;
typedef void (*MeshDrawFunc)(Mesh* mesh, mat4* modelMatrices, size_t instanceCount);
typedef void (*MeshDestroyFunc)(Mesh* mesh);

struct Mesh {
    MeshDrawFunc drawFunc;
    MeshDestroyFunc destroyFunc;

    unsigned int VAO, VBO, EBO;
    unsigned int instanceSSBO;
    size_t vertexCount;
    size_t indexCount;
};

#endif
