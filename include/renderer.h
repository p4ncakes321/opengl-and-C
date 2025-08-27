#ifndef RENDERER_H
#define RENDERER_H
#include "camera.h"
#define MAX_RENDER_PASSES 8

#include "renderpass.h"

typedef struct Renderer {
    RenderPass* passes[MAX_RENDER_PASSES];
    size_t passCount;
} Renderer;

void RendererAddPass(Renderer* renderer, RenderPass* pass);
void RendererRenderFrame(Renderer* renderer, Camera* camera);
void RendererCleanUp(Renderer* renderer);
void RendererResize(Renderer* renderer, int screen_width, int screen_height);

#endif
