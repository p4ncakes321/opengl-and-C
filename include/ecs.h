#ifndef ECS_H
#define ECS_H

#include "components/materialComponent.h"
#include "ecscomponents.h"
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

ECS* ECS_Create();
void ECS_Destroy(ECS* ecs);

Entity ECS_CreateEntity(ECS* ecs);
void ECS_DestroyEntity(ECS* ecs, Entity e);

void ECS_AddTransform(ECS* ecs, Entity e, TransformComponent* comp);
TransformComponent* ECS_GetTransform(ECS* ecs, Entity e);

void ECS_AddMaterial(ECS* ecs, Entity e, MaterialComponent* comp);
MaterialComponent* ECS_GetMaterial(ECS* ecs, Entity e);

void ECS_AddMesh(ECS* ecs, Entity e, MeshComponent* comp);
MeshComponent* ECS_GetMesh(ECS* ecs, Entity e);

#endif
