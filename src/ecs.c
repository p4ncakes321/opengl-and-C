#include "ecs.h"
#include "components/materialComponent.h"
#include <stdlib.h>

ECS* ECS_Create() {
    ECS* ecs = malloc(sizeof(ECS));
    if (!ecs) return NULL;

    ecs->nextEntity = 0;
    ecs->entities   = EntityVector_create();
    ecs->transforms = TransformComponentStorage_create();
    ecs->materials = MaterialComponentStorage_create();
    ecs->meshes     = MeshComponentStorage_create();
    return ecs;
}

void ECS_Destroy(ECS* ecs) {
    if (!ecs) return;

    TransformComponentStorage_free(ecs->transforms);
    MeshComponentStorage_free(ecs->meshes);
    EntityVector_free(ecs->entities);
    free(ecs);
}

Entity ECS_CreateEntity(ECS* ecs) {
    Entity e = ecs->nextEntity++;
    EntityVector_push(ecs->entities, e);
    return e;
}

void ECS_DestroyEntity(ECS* ecs, Entity e) {
    TransformComponentStorage_remove(ecs->transforms, e);
    MeshComponentStorage_remove(ecs->meshes, e);

    for (size_t i = 0; i < ecs->entities->count; i++) {
        if (ecs->entities->data[i] == e) {
            ecs->entities->data[i] = ecs->entities->data[ecs->entities->count - 1];
            ecs->entities->count--;
            return;
        }
    }
}

void ECS_AddTransform(ECS* ecs, Entity e, TransformComponent* comp) {
    TransformComponentStorage_add(ecs->transforms, e, comp);
}

TransformComponent* ECS_GetTransform(ECS* ecs, Entity e) {
    TransformComponent** ptr = TransformComponentStorage_get(ecs->transforms, e);
    return ptr ? *ptr : NULL;
}

void ECS_AddMaterial(ECS* ecs, Entity e, MaterialComponent* comp) {
    MaterialComponentStorage_add(ecs->materials, e, comp);
}

MaterialComponent* ECS_GetMaterial(ECS* ecs, Entity e) {
    MaterialComponent** ptr = MaterialComponentStorage_get(ecs->materials, e);
    return ptr ? *ptr : NULL;
}

void ECS_AddMesh(ECS* ecs, Entity e, MeshComponent* comp) {
    MeshComponentStorage_add(ecs->meshes, e, comp);
}

MeshComponent* ECS_GetMesh(ECS* ecs, Entity e) {
    MeshComponent** ptr = MeshComponentStorage_get(ecs->meshes, e);
    return ptr ? *ptr : NULL;
}

