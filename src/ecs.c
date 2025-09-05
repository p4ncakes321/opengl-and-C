#include "ecs.h"
#include <stdint.h>
#include <cglm/cglm.h>
#include <stdlib.h>
// another overengineered mess (but it works!?)

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
    size_t old = ecs->index_capacity;
    size_t nc = old ? old * 2 : 16;
    while (nc <= idx) nc *= 2;
    uint32_t *g = realloc(ecs->generations, nc * sizeof(uint32_t));
    if (!g) return -1;
    for (size_t i = old; i < nc; ++i) g[i] = 0;
    ecs->generations = g;
    Entity *p = realloc(ecs->parents, nc * sizeof(Entity));
    if (!p) return -1;
    for (size_t i = old; i < nc; ++i) p[i] = INVALID_ENTITY;
    ecs->parents = p;
    EntityVector **c = realloc(ecs->children, nc * sizeof(EntityVector*));
    if (!c) return -1;
    for (size_t i = old; i < nc; ++i) c[i] = NULL;
    ecs->children = c;
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
#define X(T) ecs->T##_storage = T##Storage_create((void(*)(void*))T##Destroy);
    ECS_COMPONENTS
#undef X
    ecs->parents = NULL;
    ecs->children = NULL;
    return ecs;
}

void ECS_Destroy(ECS* ecs) {
    if (!ecs) return;
#define X(T) T##Storage_free(ecs->T##_storage);
    ECS_COMPONENTS
#undef X
    if (ecs->children) {
        for (size_t i = 0; i < ecs->index_capacity; ++i) {
            if (ecs->children[i]) EntityVector_free(ecs->children[i]);
        }
    }
    free(ecs->parents);
    free(ecs->children);
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
        if (ensure_index_capacity(ecs, idx) != 0) return INVALID_ENTITY;
    }
    uint32_t gen = ecs->generations[idx];
    Entity e = MAKE_ENTITY(idx, gen);
    if (EntityList_ensure_capacity(ecs, ecs->entity_count) == 0) {
        ecs->entities[ecs->entity_count++] = e;
    }
    ecs->parents[ENTITY_INDEX(e)] = INVALID_ENTITY;
    if (ecs->children[ENTITY_INDEX(e)] == NULL) ecs->children[ENTITY_INDEX(e)] = NULL;
    return e;
}

void ECS_DestroyEntity(ECS* ecs, Entity e) {
    if (!ecs) return;
    uint32_t idx = ENTITY_INDEX(e);
    if (idx >= ecs->index_capacity) return;
    if (ecs->generations[idx] != ENTITY_GEN(e)) return;
    EntityVector* ch = ecs->children[idx];
    if (ch) {
        for (size_t i = 0; i < ch->count; ++i) {
            Entity c = ch->data[i];
            uint32_t cidx = ENTITY_INDEX(c);
            if (cidx < ecs->index_capacity) ecs->parents[cidx] = INVALID_ENTITY;
        }
        EntityVector_free(ch);
        ecs->children[idx] = NULL;
    }
    Entity parent = ecs->parents[idx];
    if (parent != INVALID_ENTITY) {
        uint32_t pidx = ENTITY_INDEX(parent);
        if (pidx < ecs->index_capacity && ecs->children[pidx]) {
            EntityVector* pv = ecs->children[pidx];
            for (size_t i = 0; i < pv->count; ++i) {
                if (pv->data[i] == e) {
                    pv->data[i] = pv->data[--pv->count];
                    break;
                }
            }
        }
    }
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
    if (!ecs) return 0;
    uint32_t idx = ENTITY_INDEX(e);
    if (idx >= ecs->index_capacity) return 0;
    return ecs->generations[idx] == ENTITY_GEN(e);
}

int ECS_SetParent(ECS* ecs, Entity child, Entity parent) {
    if (!ecs) return -1;
    if (child == parent) return -1;
    if (!Entity_IsAlive(ecs, child)) return -1;
    if (parent != INVALID_ENTITY && !Entity_IsAlive(ecs, parent)) return -1;
    uint32_t cidx = ENTITY_INDEX(child);
    Entity old = ecs->parents[cidx];
    if (old == parent) return 0;
    if (old != INVALID_ENTITY) {
        uint32_t pidx = ENTITY_INDEX(old);
        if (pidx < ecs->index_capacity && ecs->children[pidx]) {
            EntityVector* pv = ecs->children[pidx];
            for (size_t i = 0; i < pv->count; ++i) {
                if (pv->data[i] == child) {
                    pv->data[i] = pv->data[--pv->count];
                    break;
                }
            }
        }
    }
    ecs->parents[cidx] = parent;
    if (parent != INVALID_ENTITY) {
        uint32_t pidx = ENTITY_INDEX(parent);
        if (pidx >= ecs->index_capacity) return -1;
        if (!ecs->children[pidx]) {
            ecs->children[pidx] = EntityVector_create();
            if (!ecs->children[pidx]) return -1;
        }
        EntityVector_push(ecs->children[pidx], child);
    }
    return 0;
}

Entity ECS_GetParent(ECS* ecs, Entity child) {
    if (!ecs) return INVALID_ENTITY;
    uint32_t cidx = ENTITY_INDEX(child);
    if (cidx >= ecs->index_capacity) return INVALID_ENTITY;
    return ecs->parents[cidx];
}

EntityVector* ECS_GetChildren(ECS* ecs, Entity parent) {
    if (!ecs) return NULL;
    uint32_t pidx = ENTITY_INDEX(parent);
    if (pidx >= ecs->index_capacity) return NULL;
    return ecs->children[pidx];
}

mat4* ECS_GetWorldTransform(ECS* ecs, Entity e) {
    if (!ecs || !Entity_IsAlive(ecs, e)) return NULL;
    TransformComponent* tc = ECS_GetTransformComponent(ecs, e);
    if (!tc) return NULL;

    mat4 parentMat;
    Entity parent = ECS_GetParent(ecs, e);
    
    if (parent != INVALID_ENTITY) {
        TransformComponent* ptc = ECS_GetTransformComponent(ecs, parent);
        if (ptc && ptc->instanceCount > 0) {
            mat4* pWorldArr = ECS_GetWorldTransform(ecs, parent);
            if (pWorldArr) {
                size_t parentIdx = tc->inheritIndex;
                if (parentIdx >= ptc->instanceCount) parentIdx = 0;
                glm_mat4_copy(pWorldArr[parentIdx], parentMat);
            } else {
                glm_mat4_identity(parentMat);
            }
        } else {
            mat4* pWorld = ECS_GetWorldTransform(ecs, parent);
            if (pWorld) glm_mat4_copy(*pWorld, parentMat);
            else glm_mat4_identity(parentMat);
        }
    } else {
        glm_mat4_identity(parentMat);
    }

    for (size_t i = 0; i < tc->instanceCount; i++) {
        mat4 local;
        glm_translate_make(local, tc->positions[i]);
        mat4 rot;
        glm_euler_xyz(tc->rotations[i], rot);
        glm_mat4_mul(local, rot, local);
        glm_scale(local, tc->scales[i]);
        glm_mat4_mul(parentMat, local, tc->worldMatrices[i]);
    }

    return tc->worldMatrices;
}

#define X(T) \
EntityVector* ECS_GetEntitiesWith##T(ECS* ecs) { \
    if (!ecs) return NULL; \
    EntityVector* list = EntityVector_create(); \
    if (!list) return NULL; \
    for (size_t i = 0; i < ecs->entity_count; ++i) { \
        Entity e = ecs->entities[i]; \
        if (ECS_Get##T(ecs, e)) { \
            EntityVector_push(list, e); \
        } \
    } \
    return list; \
}
ECS_COMPONENTS
#undef X

static float clampf_local(float v, float lo, float hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

void ECS_UpdateCameras(ECS* ecs) {
    if (!ecs) return;

    EntityVector* cams = ECS_GetEntitiesWithCameraComponent(ecs);
    if (!cams) return;

    for (size_t i = 0; i < cams->count; ++i) {
        Entity e = cams->data[i];
        CameraComponent* cc = ECS_GetCameraComponent(ecs, e);
        if (!cc || !cc->active || !cc->camera) continue;

        mat4* worldArr = ECS_GetWorldTransform(ecs, e);
        if (!worldArr) continue;

        size_t idx = cc->instanceIndex;
        TransformComponent* tc = ECS_GetTransformComponent(ecs, e);
        mat4 worldMat;
        if (tc && idx < tc->instanceCount) {
            glm_mat4_copy(tc->worldMatrices[idx], worldMat);
        } else {
            glm_mat4_copy(worldArr[0], worldMat);
        }

        vec3 pos = { worldMat[3][0], worldMat[3][1], worldMat[3][2] };
        glm_vec3_copy(pos, cc->camera->position);
        vec4 localF = { 0.0f, 0.0f, -1.0f, 0.0f };
        vec4 localU = { 0.0f, 1.0f, 0.0f, 0.0f };
        vec4 worldF4, worldU4;
        glm_mat4_mulv(worldMat, localF, worldF4);
        glm_mat4_mulv(worldMat, localU, worldU4);

        vec3 forward = { worldF4[0], worldF4[1], worldF4[2] };
        vec3 up      = { worldU4[0], worldU4[1], worldU4[2] };

        glm_vec3_normalize(forward);
        glm_vec3_normalize(up);

        glm_vec3_copy(forward, cc->camera->forward);
        glm_vec3_copy(up, cc->camera->up);

        glm_vec3_cross(cc->camera->forward, cc->camera->up, cc->camera->right);
        glm_vec3_normalize(cc->camera->right);

        glm_vec3_copy(cc->camera->up, cc->camera->worldUp);

        float fy = clampf_local(cc->camera->forward[1], -1.0f, 1.0f);
        cc->camera->pitch = glm_deg(asinf(fy));
        cc->camera->yaw   = glm_deg(atan2f(cc->camera->forward[0], cc->camera->forward[2]));
    }

    EntityVector_free(cams);
}
