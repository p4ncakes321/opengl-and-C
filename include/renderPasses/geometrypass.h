#ifndef GEOMPASS_H
#define GEOMPASS_H

#include "renderpass.h"

typedef struct GeometryPass {
    RenderPass base;
} GeometryPass;

GeometryPass* GeometryPassCreate();

#endif
