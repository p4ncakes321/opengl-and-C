#ifndef DEFMAT_H
#define DEFMAT_H

#include "material.h"
#include "texture.h"

typedef struct DefaultMaterial {
    Material base;
    Texture* albedo;
} DefaultMaterial;

DefaultMaterial* DefaultMaterialCreate(const char* albedoPath);

#endif
