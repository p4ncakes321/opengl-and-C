#ifndef GEOMPASS_H
#define GEOMPASS_H

#include "renderpass.h"
#include "vector.h"
#include "material.h"
#include "mesh.h"
#include <cglm/cglm.h>

typedef struct RenderObject {
    Mesh* mesh;
    Material* material; 
    mat4* modelMatrix;
    size_t instanceCount;
} RenderObject;

DEFINE_VECTOR(RenderObject, ObjectVector)

typedef struct GeometryPass {
    RenderPass base;
    ObjectVector* objects; 
} GeometryPass;

GeometryPass* GeometryPassCreate();
void GeometryPassAddObject(GeometryPass* pass, RenderObject object);
void GeometryPassClearObjects(GeometryPass* pass);

RenderObject RenderObjectCreate(Mesh* mesh, Material* material, mat4* modelMatrix, size_t instanceCount);

#endif
