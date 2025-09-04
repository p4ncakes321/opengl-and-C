#ifndef ECS_H
#define ECS_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "vector.h"
#include "components/transformcomponent.h"
#include "components/materialcomponent.h"
#include "components/meshcomponent.h"
#include "components/cameracomponent.h"

typedef uint32_t Entity;
DEFINE_VECTOR(Entity, EntityVector);

#define INVALID_ENTITY ((Entity)UINT32_MAX)

#define INDEX_BITS 24u
#define INDEX_MASK ((1u << INDEX_BITS) - 1u)
#define GENERATION_BITS (32u - INDEX_BITS)
#define GEN_MASK ((1u << GENERATION_BITS) - 1u)

#define MAKE_ENTITY(index, gen) (Entity)((((uint32_t)(gen) & GEN_MASK) << INDEX_BITS) | ((uint32_t)(index) & INDEX_MASK))
#define ENTITY_INDEX(e) ((uint32_t)((e) & INDEX_MASK))
#define ENTITY_GEN(e)   ((uint32_t)((e) >> INDEX_BITS))

#define ECS_COMPONENTS \
    X(TransformComponent) \
    X(MaterialComponent)  \
    X(MeshComponent) \
    X(CameraComponent)   

#define DEFINE_COMPONENT_STORAGE(Type)                               \
    typedef void (*Type##Destructor)(void*);                         \
    typedef struct {                                                 \
        Type **items;                                                \
        size_t capacity;                                             \
        Type##Destructor destroy;                                    \
    } Type##Storage;                                                 \
                                                                    \
    static inline Type##Storage* Type##Storage_create(Type##Destructor dtor) { \
        Type##Storage* s = malloc(sizeof(*s));                       \
        if (!s) return NULL;                                         \
        s->items = NULL;                                             \
        s->capacity = 0;                                             \
        s->destroy = dtor;                                           \
        return s;                                                    \
    }                                                                \
                                                                    \
    static inline void Type##Storage_free(Type##Storage* s) {        \
        if (!s) return;                                              \
        for (size_t i = 0; i < s->capacity; ++i) {                   \
            if (s->items[i]) {                                       \
                if (s->destroy) s->destroy(s->items[i]);             \
                free(s->items[i]);                                   \
            }                                                        \
        }                                                            \
        free(s->items);                                              \
        free(s);                                                     \
    }                                                                \
                                                                    \
    static inline int Type##Storage_ensure(Type##Storage* s, size_t idx) { \
        if (idx < s->capacity) return 0;                             \
        size_t newcap = s->capacity ? s->capacity * 2 : 16;          \
        while (newcap <= idx) newcap *= 2;                           \
        Type **tmp = realloc(s->items, newcap * sizeof(Type*));      \
        if (!tmp) return -1;                                         \
        for (size_t i = s->capacity; i < newcap; ++i) tmp[i] = NULL; \
        s->items = tmp;                                              \
        s->capacity = newcap;                                        \
        return 0;                                                    \
    }                                                                \
                                                                    \
    static inline int Type##Storage_add(Type##Storage* s, Entity e, Type* comp) { \
        if (!s) return -1;                                           \
        size_t idx = ENTITY_INDEX(e);                                \
        if (Type##Storage_ensure(s, idx) != 0) return -1;            \
        if (s->items[idx]) {                                         \
            if (s->destroy) s->destroy(s->items[idx]);               \
            free(s->items[idx]);                                     \
        }                                                            \
        s->items[idx] = comp;                                        \
        return 0;                                                    \
    }                                                                \
                                                                    \
    static inline Type* Type##Storage_get(Type##Storage* s, Entity e) { \
        if (!s) return NULL;                                         \
        size_t idx = ENTITY_INDEX(e);                                \
        if (idx >= s->capacity) return NULL;                         \
        return s->items[idx];                                        \
    }                                                                \
                                                                    \
    static inline void Type##Storage_remove(Type##Storage* s, Entity e) { \
        if (!s) return;                                              \
        size_t idx = ENTITY_INDEX(e);                                \
        if (idx >= s->capacity) return;                              \
        if (s->items[idx]) {                                         \
            if (s->destroy) s->destroy(s->items[idx]);               \
            free(s->items[idx]);                                     \
            s->items[idx] = NULL;                                    \
        }                                                            \
    }

#define X(T) DEFINE_COMPONENT_STORAGE(T)
ECS_COMPONENTS
#undef X

typedef struct ECS {
    uint32_t next_index;
    uint32_t *free_indices;
    size_t free_count;
    size_t free_capacity;
    uint32_t *generations;
    size_t index_capacity;
    Entity *entities;
    size_t entity_count;
    size_t entity_capacity;
#define X(T) T##Storage* T##_storage;
    ECS_COMPONENTS
#undef X
    Entity *parents;
    EntityVector **children;
} ECS;

int EntityList_ensure_capacity(ECS* ecs, size_t idx);

#define X(T) \
    int ECS_Add##T(ECS* ecs, Entity e, T* comp); \
    T* ECS_Get##T(ECS* ecs, Entity e); \
    void ECS_Remove##T(ECS* ecs, Entity e);
ECS_COMPONENTS
#undef X

#define X(T) EntityVector* ECS_GetEntitiesWith##T(ECS* ecs);
ECS_COMPONENTS
#undef X

ECS* ECS_Create(void);
void ECS_Destroy(ECS* ecs);
Entity ECS_CreateEntity(ECS* ecs);
void ECS_DestroyEntity(ECS* ecs, Entity e);
int Entity_IsAlive(ECS* ecs, Entity e);

int ECS_SetParent(ECS* ecs, Entity child, Entity parent);
Entity ECS_GetParent(ECS* ecs, Entity child);
EntityVector* ECS_GetChildren(ECS* ecs, Entity parent);

mat4* ECS_GetWorldTransform(ECS* ecs, Entity e);
void ECS_UpdateCameras(ECS* ecs);

#endif
