#ifndef RENDERER_H
#define RENDERER_H

#include "camera.h"
#include "renderpass.h"
#include "vector.h"

DEFINE_VECTOR(CameraView*, CameraViewVector)
DEFINE_VECTOR(RenderPass*, RenderPassVector)

typedef struct Renderer {
    RenderPassVector* passes;
} Renderer;

void RendererAddPass(Renderer* renderer, RenderPass* pass);
void RendererRenderFrame(Renderer* renderer, CameraViewVector* camera);
void RendererFree(Renderer* renderer);
void RendererResize(Renderer* renderer, int screen_width, int screen_height);

#endif
