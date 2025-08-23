#include "mesh.h"
#include "glad/glad.h"
#include <GL/gl.h>
#include <string.h>

Mesh* MeshCreate(Vertex* vertices, size_t vertexCount, GLuint* indices, size_t indexCount) {
    Mesh* mesh = (Mesh*)malloc(sizeof(Mesh));
    if (!mesh) return NULL;
    mesh->vertexCount = vertexCount;
    mesh->indexCount = indexCount;

    mesh->vertices = malloc(sizeof(Vertex) * vertexCount);
    memcpy(mesh->vertices, vertices, sizeof(Vertex) * vertexCount);

    mesh->indices = malloc(sizeof(GLuint) * indexCount);
    memcpy(mesh->indices, indices, sizeof(GLuint) * indexCount);
    
    glGenVertexArrays(1, &mesh->VAO);
    glGenBuffers(1, &mesh->VBO);
    glGenBuffers(1, &mesh->EBO);

    glBindVertexArray(mesh->VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(GLuint), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
    glEnableVertexAttribArray(3);

    glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv2));
    glEnableVertexAttribArray(4);

    glBindVertexArray(0);

    return mesh;
}

void MeshDraw(Mesh *mesh) {
    glBindVertexArray(mesh->VAO);
    glDrawElements(GL_TRIANGLES, mesh->indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}


void MeshDestroy(Mesh *mesh) {
    if (!mesh) return;

    glDeleteVertexArrays(1, &mesh->VAO);
    glDeleteBuffers(1, &mesh->VBO);
    glDeleteBuffers(1, &mesh->EBO);

    free(mesh->vertices);
    free(mesh->indices);
    free(mesh);
}
