#ifndef VECTOR_H
#define VECTOR_H

#define DEFINE_VECTOR(type, name)                                         \
typedef struct {                                                              \
    type* data;                                                               \
    size_t count;                                                             \
    size_t capacity;                                                          \
} name;                                                                       \
                                                                              \
static inline name* name##_create() {                                         \
    name* arr = malloc(sizeof(name));                                         \
    arr->count = 0;                                                           \
    arr->capacity = 64;                                                       \
    arr->data = malloc(arr->capacity * sizeof(type));                         \
    return arr;                                                               \
}                                                                             \
                                                                              \
static inline void name##_free(name* arr) {                                   \
    free(arr->data);                                                          \
    free(arr);                                                                \
}                                                                             \
                                                                              \
static inline void name##_clear(name* arr) {                                  \
    arr->count = 0;                                                           \
}                                                                             \
                                                                              \
static inline void name##_push(name* arr, type value) {                       \
    if (arr->count >= arr->capacity) {                                        \
        arr->capacity *= 2;                                                   \
        arr->data = realloc(arr->data, arr->capacity * sizeof(type));         \
    }                                                                         \
    arr->data[arr->count++] = value;                                          \
}                                                                             \
                                                                              \
static inline type* name##_get(name* arr, size_t index) {                     \
    return &arr->data[index];                                                 \
}


#endif

