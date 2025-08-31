#ifndef ECS_COMPONENTS_H
#define ECS_COMPONENTS_H

#include "vector.h"
#include <stdlib.h>
#include <stdint.h>

typedef uint32_t Entity;

#define DEFINE_COMPONENT_STORAGE(Type, Name)                        \
    DEFINE_VECTOR(Type*, Name##_Vector)                              \
    typedef struct {                                                \
        Name##_Vector* components;                                  \
        EntityVector* entities;                                     \
    } Name##Storage;                                                \
                                                                    \
    static inline Name##Storage* Name##Storage_create() {           \
        Name##Storage* s = malloc(sizeof(Name##Storage));           \
        if (!s) return NULL;                                        \
        s->components = Name##_Vector_create();                     \
        s->entities   = EntityVector_create();                      \
        return s;                                                   \
    }                                                               \
                                                                    \
    static inline void Name##Storage_free(Name##Storage* s) {       \
        if (!s) return;                                             \
        for (size_t i = 0; i < s->components->count; i++) {        \
            free(s->components->data[i]);                           \
        }                                                           \
        Name##_Vector_free(s->components);                           \
        EntityVector_free(s->entities);                              \
        free(s);                                                    \
    }                                                               \
                                                                    \
    static inline void Name##Storage_add(Name##Storage* s, Entity e, Type* comp) { \
        Name##_Vector_push(s->components, comp);                    \
        EntityVector_push(s->entities, e);                          \
    }                                                               \
                                                                    \
    static inline Type** Name##Storage_get(Name##Storage* s, Entity e) { \
        for (size_t i = 0; i < s->entities->count; i++) {          \
            if (s->entities->data[i] == e) return &s->components->data[i]; \
        }                                                           \
        return NULL;                                                \
    }                                                               \
                                                                    \
    static inline void Name##Storage_remove(Name##Storage* s, Entity e) { \
        for (size_t i = 0; i < s->entities->count; i++) {          \
            if (s->entities->data[i] == e) {                        \
                free(s->components->data[i]);                       \
                s->components->data[i] = s->components->data[s->components->count - 1]; \
                s->entities->data[i]   = s->entities->data[s->entities->count - 1]; \
                s->components->count--;                              \
                s->entities->count--;                                 \
                return;                                             \
            }                                                       \
        }                                                           \
    }

#endif

