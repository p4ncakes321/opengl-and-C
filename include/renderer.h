#ifndef RENDERER_H
#define RENDERER_H
#define MAX_RENDER_PASSES 8

#include "renderpass.h"

typedef struct Renderer {
    RenderPass* passes[MAX_RENDER_PASSES];
    size_t passCount;
} Renderer;

void RendererAddPass(Renderer* renderer, RenderPass* pass);
void RendererRenderFrame(Renderer* renderer); // will add more stuff as i expand the engine (camera, scene)
void RendererCleanUp(Renderer* renderer);

#endif
