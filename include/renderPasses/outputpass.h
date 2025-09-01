#ifndef OUTPUTPASS_H
#define OUTPUTPASS_H

#include "renderpass.h"
#include "gbuffer.h"
#include "shader.h"
#include <glad/glad.h>

typedef struct OutputPass {
    RenderPass base;

    Shader shader;
    GLuint quadVAO;
    GLuint quadVBO;

    GBuffer* gbuffer;
} OutputPass;

OutputPass* OutputPassCreate(GBuffer* gbuffer);
void OutputPassDestroy(RenderPass* pass);

#endif
