#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>

#define DEFINE_VECTOR(type, name)                                         \
typedef struct {                                                          \
    type* data;                                                           \
    size_t count;                                                         \
    size_t capacity;                                                      \
} name;                                                                    \
                                                                          \
static inline name* name##_create() {                                     \
    name* arr = malloc(sizeof(name));                                     \
    if (!arr) return NULL;                                                \
    arr->count = 0;                                                       \
    arr->capacity = 8;                                                      \
    arr->data = malloc(arr->capacity * sizeof(type));                     \
    if (!arr->data) { free(arr); return NULL; }                           \
    return arr;                                                           \
}                                                                         \
                                                                          \
static inline void name##_free(name* arr) {                               \
    if (!arr) return;                                                     \
    free(arr->data);                                                      \
    free(arr);                                                            \
}                                                                         \
                                                                          \
static inline void name##_clear(name* arr) {                              \
    arr->count = 0;                                                       \
}                                                                         \
                                                                          \
static inline int name##_push(name* arr, type value) {                     \
    if (arr->count >= arr->capacity) {                                    \
        size_t new_capacity = arr->capacity + arr->capacity / 2;         \
        type* tmp = realloc(arr->data, new_capacity * sizeof(type));      \
        if (!tmp) return 0;                                                \
        arr->data = tmp;                                                  \
        arr->capacity = new_capacity;                                     \
    }                                                                     \
    arr->data[arr->count++] = value;                                      \
    return 1;                                                             \
}                                                                         \
                                                                          \
static inline type* name##_get(name* arr, size_t index) {                 \
    return index < arr->count ? &arr->data[index] : NULL;                 \
}

#endif

