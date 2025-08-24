#ifndef MATERIAL_H
#define MATERIAL_H

#include "shader.h"

typedef struct UniversalMaterial UniversalMaterial;
typedef void (*BindFunc)(UniversalMaterial* mat, bool isForward);
typedef void (*MatDestroyFunc)(UniversalMaterial* mat);

typedef struct BaseMaterial {
    BindFunc bindFunc;
    MatDestroyFunc destroyFunc;

    Shader deferredShader;
    Shader forwardShader;
} BaseMaterial;

#endif

