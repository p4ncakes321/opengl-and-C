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

typedef struct Mesh {
    unsigned int VAO, VBO, EBO;
    size_t vertexCount;
    size_t indexCount;

    Vertex* vertices;
    GLuint* indices;
} Mesh;

Mesh* MeshCreate(Vertex* vertices, size_t vertexCount, GLuint* indices, size_t indexCount);
void MeshDraw(Mesh* mesh);
void MeshDestroy(Mesh* mesh);

#endif
