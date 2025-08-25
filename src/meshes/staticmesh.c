#include "meshes/staticmesh.h"
#include <stdlib.h>
#include <string.h>
#include <glad/glad.h>

static void StaticMeshDraw(Mesh* mesh, mat4* modelMatrices, size_t instanceCount) {
    glBindVertexArray(mesh->VAO);
    if (instanceCount > 1 && modelMatrices) {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, mesh->instanceSSBO);
        glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(mat4) * instanceCount, modelMatrices, GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, mesh->instanceSSBO); // binding point must match shader

        glDrawElementsInstanced(GL_TRIANGLES, mesh->indexCount, GL_UNSIGNED_INT, 0, instanceCount);
    } else if (instanceCount == 1 && modelMatrices) {
        glDrawElements(GL_TRIANGLES, mesh->indexCount, GL_UNSIGNED_INT, 0);
    } else {
        glDrawElements(GL_TRIANGLES, mesh->indexCount, GL_UNSIGNED_INT, 0);
    }
}

static void StaticMeshDestroy(Mesh* mesh) {
    StaticMesh* sm = (StaticMesh*)mesh;
    if (sm->vertices) free(sm->vertices);
    if (sm->indices) free(sm->indices);
    glDeleteBuffers(1, &mesh->VBO);
    glDeleteBuffers(1, &mesh->EBO);
    glDeleteVertexArrays(1, &mesh->VAO);
    free(sm);
}

StaticMesh* StaticMeshCreate(Vertex* vertices, size_t vertexCount, GLuint* indices, size_t indexCount) {
    StaticMesh* sm = (StaticMesh*)malloc(sizeof(StaticMesh));
    if (!sm) return NULL;

    sm->vertices = (Vertex*)malloc(sizeof(Vertex) * vertexCount);
    sm->indices  = (GLuint*)malloc(sizeof(GLuint) * indexCount);
    memcpy(sm->vertices, vertices, sizeof(Vertex) * vertexCount);
    memcpy(sm->indices, indices, sizeof(GLuint) * indexCount);

    sm->base.vertexCount = vertexCount;
    sm->base.indexCount  = indexCount;
    sm->base.drawFunc    = StaticMeshDraw;
    sm->base.destroyFunc = StaticMeshDestroy;

    glGenVertexArrays(1, &sm->base.VAO);
    glGenBuffers(1, &sm->base.VBO);
    glGenBuffers(1, &sm->base.EBO);

    glBindVertexArray(sm->base.VAO);

    glGenBuffers(1, &sm->base.instanceSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, sm->base.instanceSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(mat4) * 1, NULL, GL_DYNAMIC_DRAW); 
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, sm->base.instanceSSBO); 

    glBindBuffer(GL_ARRAY_BUFFER, sm->base.VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex), sm->vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sm->base.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(GLuint), sm->indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv2));

    glBindVertexArray(0);

    return sm;
}

