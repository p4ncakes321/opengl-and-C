#ifndef MAT_COMP_H
#define MAT_COMP_H

#include "material.h"
#include <stddef.h>

typedef struct MaterialComponent {
    Material* material;
} MaterialComponent;

MaterialComponent* MaterialComponentCreate(Material* material);
void MaterialComponentSetMaterial(MaterialComponent* component, Material* material);
void MaterialComponentDestroy(MaterialComponent* component);

#endif

