#include "ecs.h"

int EntityList_ensure_capacity(ECS* ecs, size_t idx) {
    if (idx < ecs->entity_capacity) return 0;
    size_t nc = ecs->entity_capacity ? ecs->entity_capacity * 2 : 16;
    while (nc <= idx) nc *= 2;
    Entity *tmp = realloc(ecs->entities, nc * sizeof(Entity));
    if (!tmp) return -1;
    ecs->entities = tmp;
    ecs->entity_capacity = nc;
    return 0;
}

#define X(T) \
    int ECS_Add##T(ECS* ecs, Entity e, T* comp) { return T##Storage_add(ecs->T##_storage, e, comp); } \
    T* ECS_Get##T(ECS* ecs, Entity e) { return T##Storage_get(ecs->T##_storage, e); } \
    void ECS_Remove##T(ECS* ecs, Entity e) { T##Storage_remove(ecs->T##_storage, e); }
ECS_COMPONENTS
#undef X

static int ensure_index_capacity(ECS *ecs, uint32_t idx) {
    if (idx < ecs->index_capacity) return 0;
    size_t nc = ecs->index_capacity ? ecs->index_capacity * 2 : 16;
    while (nc <= idx) nc *= 2;
    uint32_t *g = realloc(ecs->generations, nc * sizeof(uint32_t));
    if (!g) return -1;
    for (size_t i = ecs->index_capacity; i < nc; ++i) g[i] = 0;
    ecs->generations = g;
    ecs->index_capacity = nc;
    return 0;
}

static int push_free_index(ECS *ecs, uint32_t idx) {
    if (ecs->free_count == ecs->free_capacity) {
        size_t nc = ecs->free_capacity ? ecs->free_capacity * 2 : 16;
        uint32_t *tmp = realloc(ecs->free_indices, nc * sizeof(uint32_t));
        if (!tmp) return -1;
        ecs->free_indices = tmp;
        ecs->free_capacity = nc;
    }
    ecs->free_indices[ecs->free_count++] = idx;
    return 0;
}

ECS* ECS_Create(void) {
    ECS* ecs = malloc(sizeof(*ecs));
    if (!ecs) return NULL;
    ecs->next_index = 0;
    ecs->free_indices = NULL;
    ecs->free_count = 0;
    ecs->free_capacity = 0;
    ecs->generations = NULL;
    ecs->index_capacity = 0;
    ecs->entities = NULL;
    ecs->entity_count = 0;
    ecs->entity_capacity = 0;
#define X(T) ecs->T##_storage = T##Storage_create();
    ECS_COMPONENTS
#undef X
    return ecs;
}

void ECS_Destroy(ECS* ecs) {
    if (!ecs) return;
#define X(T) T##Storage_free(ecs->T##_storage);
    ECS_COMPONENTS
#undef X
    free(ecs->free_indices);
    free(ecs->generations);
    free(ecs->entities);
    free(ecs);
}

Entity ECS_CreateEntity(ECS* ecs) {
    uint32_t idx;
    if (ecs->free_count > 0) {
        idx = ecs->free_indices[--ecs->free_count];
    } else {
        idx = ecs->next_index++;
        if (ensure_index_capacity(ecs, idx) != 0) return UINT32_MAX;
    }
    uint32_t gen = ecs->generations[idx];
    Entity e = MAKE_ENTITY(idx, gen);
    if (EntityList_ensure_capacity(ecs, ecs->entity_count) == 0) {
        ecs->entities[ecs->entity_count++] = e;
    }
    return e;
}

void ECS_DestroyEntity(ECS* ecs, Entity e) {
    uint32_t idx = ENTITY_INDEX(e);
    if (idx >= ecs->index_capacity) return;
    if (ecs->generations[idx] != ENTITY_GEN(e)) return;
#define X(T) ECS_Remove##T(ecs, e);
    ECS_COMPONENTS
#undef X
    ecs->generations[idx] = (ecs->generations[idx] + 1) & (uint32_t)GEN_MASK;
    push_free_index(ecs, idx);
    for (size_t i = 0; i < ecs->entity_count; ++i) {
        if (ecs->entities[i] == e) {
            ecs->entities[i] = ecs->entities[ecs->entity_count - 1];
            ecs->entity_count--;
            break;
        }
    }
}

int Entity_IsAlive(ECS* ecs, Entity e) {
    uint32_t idx = ENTITY_INDEX(e);
    if (idx >= ecs->index_capacity) return 0;
    return ecs->generations[idx] == ENTITY_GEN(e);
}

