#ifndef ENGINE_H
#define ENGINE_H
#include <stdbool.h>

typedef struct Engine {
    bool initializedGlad;
} Engine;

void EngineInit(void);
void EnginePollEvents(void);
bool EngineInitializeGlad(void);
double EngineGetTime(void);
void EngineTerminate(void);

#endif 

