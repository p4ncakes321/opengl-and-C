#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <stddef.h>
#include <stdbool.h>
#include "vector.h" // wont shut up about that its not used but when i remove it, it screams about not having it 😔
#include "mesh.h"
#include "material.h"

#define RESOURCE_TYPES \
    X(Mesh)            \
    X(Material)

#define X(T) \
typedef struct { char* key; T* ptr; } T##Entry; \
DEFINE_VECTOR(T##Entry, T##EntryVector)
RESOURCE_TYPES
#undef X

typedef struct ResourceManager {
#define X(T) T##EntryVector* T##Entries;
RESOURCE_TYPES
#undef X
} ResourceManager;

ResourceManager* ResourceManagerCreate(void);
void ResourceManagerFree(ResourceManager* resourceManager);

#define X(T) \
T* ResourceManager_Register##T(ResourceManager* rm, const char* key, T* obj); \
T* ResourceManager_Get##T(ResourceManager* rm, const char* key); \
bool ResourceManager_Remove##T(ResourceManager* rm, const char* key);
RESOURCE_TYPES
#undef X

#endif

