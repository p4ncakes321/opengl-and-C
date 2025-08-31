#include "components/materialComponent.h"
#include <stdlib.h>

MaterialComponent* MaterialComponentCreate(Material* material) {
    MaterialComponent* component = (MaterialComponent*)malloc(sizeof(MaterialComponent));
    component->material = material;
    return component;
}

void MaterialComponentSetMaterial(MaterialComponent* component, Material* material) {
    if (!component) return;
    component->material = material;
}
