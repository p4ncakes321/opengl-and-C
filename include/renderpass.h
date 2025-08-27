#ifndef RENDERPASS_H
#define RENDERPASS_H

#include "camera.h"
#include "glad/glad.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct RenderPass RenderPass;
typedef void (*renderMethod)(RenderPass* self, RenderPass* lastPass, Camera* camera); 
typedef void (*destroyMethod)(RenderPass* self);

typedef struct RenderPass {
    int screen_width, screen_height;
    renderMethod render;
    destroyMethod destroy;

    const char* name;
    
    GLuint outputFramebuffer;
    GLuint* outputTextures;
    size_t outputCount;
} RenderPass;

#endif
