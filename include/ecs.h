#ifndef ECS_H
#define ECS_H

#include "components/materialComponent.h"
#include "ecscomponents.h"
#include "ecscomponentmethods.h"
#include "components/transformcomponent.h"
#include "components/meshcomponent.h"

DEFINE_VECTOR(Entity, EntityVector)
DEFINE_COMPONENT_STORAGE(TransformComponent, TransformComponent)
DEFINE_COMPONENT_STORAGE(MaterialComponent, MaterialComponent);
DEFINE_COMPONENT_STORAGE(MeshComponent, MeshComponent)

typedef struct ECS {
    Entity nextEntity;
    EntityVector* entities;

    TransformComponentStorage* transforms;
    MaterialComponentStorage* materials;
    MeshComponentStorage* meshes;
} ECS;

DEFINE_ECS_COMPONENT_METHODS(TransformComponent, transforms);
DEFINE_ECS_COMPONENT_METHODS(MaterialComponent, materials);
DEFINE_ECS_COMPONENT_METHODS(MeshComponent, meshes);

ECS* ECS_Create();
void ECS_Destroy(ECS* ecs);

Entity ECS_CreateEntity(ECS* ecs);
void ECS_DestroyEntity(ECS* ecs, Entity e);

#endif
