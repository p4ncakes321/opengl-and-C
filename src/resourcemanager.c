#include "resourcemanager.h"
#include <stdlib.h>
#include <string.h>
// OVERENGINEERED MESS
static char* str_dup_safe(const char* s) {
    if (!s) return NULL;
    size_t l = strlen(s) + 1;
    char* d = malloc(l);
    if (!d) return NULL;
    memcpy(d, s, l);
    return d;
}

ResourceManager* ResourceManagerCreate(void) {
    ResourceManager* rm = malloc(sizeof(ResourceManager));
    if (!rm) return NULL;
#define X(T) rm->T##Entries = T##EntryVector_create();
    RESOURCE_TYPES
#undef X
    return rm;
}

void ResourceManagerFree(ResourceManager* resourceManager) {
    if (!resourceManager) return;
#define X(T) \
    if (resourceManager->T##Entries) { \
        for (size_t i = 0; i < resourceManager->T##Entries->count; ++i) { \
            T##Entry* e = &resourceManager->T##Entries->data[i]; \
            if (e->ptr) { if (e->ptr->destroyFunc) e->ptr->destroyFunc(e->ptr); } \
            free(e->key); \
        } \
        T##EntryVector_free(resourceManager->T##Entries); \
        resourceManager->T##Entries = NULL; \
    }
    RESOURCE_TYPES
#undef X
    free(resourceManager);
}

#define X(T) \
static long find_##T##_index(ResourceManager* rm, const char* key) { \
    if (!rm || !rm->T##Entries || !key) return -1; \
    for (size_t i = 0; i < rm->T##Entries->count; ++i) { \
        char* k = rm->T##Entries->data[i].key; \
        if (k && strcmp(k, key) == 0) return (long)i; \
    } \
    return -1; \
} \
\
T* ResourceManager_Register##T(ResourceManager* rm, const char* key, T* obj) { \
    if (!rm || !key || !obj) return NULL; \
    if (!rm->T##Entries) rm->T##Entries = T##EntryVector_create(); \
    long idx = find_##T##_index(rm, key); \
    if (idx >= 0) { \
        T* existing = rm->T##Entries->data[idx].ptr; \
        if (obj && obj->destroyFunc) obj->destroyFunc(obj); \
        return existing; \
    } \
    T##Entry e; \
    e.key = str_dup_safe(key); \
    e.ptr = obj; \
    if (!e.key) { if (obj && obj->destroyFunc) obj->destroyFunc(obj); return NULL; } \
    if (!T##EntryVector_push(rm->T##Entries, e)) { free(e.key); if (obj && obj->destroyFunc) obj->destroyFunc(obj); return NULL; } \
    return obj; \
} \
\
T* ResourceManager_Get##T(ResourceManager* rm, const char* key) { \
    if (!rm || !rm->T##Entries || !key) return NULL; \
    long idx = find_##T##_index(rm, key); \
    if (idx < 0) return NULL; \
    return rm->T##Entries->data[idx].ptr; \
} \
\
bool ResourceManager_Remove##T(ResourceManager* rm, const char* key) { \
    if (!rm || !rm->T##Entries || !key) return false; \
    long idx = find_##T##_index(rm, key); \
    if (idx < 0) return false; \
    size_t i = (size_t)idx; \
    T##Entry entry = rm->T##Entries->data[i]; \
    if (entry.ptr && entry.ptr->destroyFunc) entry.ptr->destroyFunc(entry.ptr); \
    free(entry.key); \
    size_t last = rm->T##Entries->count - 1; \
    if (i != last) rm->T##Entries->data[i] = rm->T##Entries->data[last]; \
    rm->T##Entries->count--; \
    return true; \
}
RESOURCE_TYPES
#undef X

