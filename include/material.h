#ifndef MATERIAL_H
#define MATERIAL_H

#include "camera.h"
#include "shader.h"
#include <cglm/cglm.h>

typedef struct Material Material;
typedef void (*BindFunc)(Material* mat, mat4* modelMatrix, size_t instanceCount, Camera* camera, bool isForward);
typedef void (*MatDestroyFunc)(Material* mat);

typedef struct Material {
    BindFunc bindFunc;
    MatDestroyFunc destroyFunc;

    Shader deferredShader;
    Shader forwardShader;
} Material;

#endif

