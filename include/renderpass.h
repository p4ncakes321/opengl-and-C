#ifndef RENDERPASS_H
#define RENDERPASS_H

#include "glad/glad.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct RenderPass RenderPass;
typedef void (*renderMethod)(RenderPass* self, RenderPass* lastPass) ;// add camera and other stuff here later
typedef void (*destroyMethod)(RenderPass* self);

typedef struct RenderPass {
    renderMethod render;
    destroyMethod destroy;

    const char* name;
    
    GLuint outputFramebuffer;
    GLuint* outputTextures;
    size_t outputCount;
} RenderPass;


#endif
