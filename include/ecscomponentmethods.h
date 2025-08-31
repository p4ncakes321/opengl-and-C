#ifndef ECS_COMPONENT_METHODS_H
#define ECS_COMPONENT_METHODS_H

#define DEFINE_ECS_COMPONENT_METHODS(Type, field) \
    static inline void ECS_Add##Type(ECS* ecs, Entity e, Type* comp) { \
        Type##Storage_add(ecs->field, e, comp); \
    } \
    static inline Type* ECS_Get##Type(ECS* ecs, Entity e) { \
        Type** ptr = Type##Storage_get(ecs->field, e); \
        return ptr ? *ptr : NULL; \
    } \
    static inline void ECS_Remove##Type(ECS* ecs, Entity e) { \
        Type##Storage_remove(ecs->field, e); \
    }

#endif
