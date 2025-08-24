#include "renderer.h"
#include "renderpass.h"
#include <stdio.h>

void RendererAddPass(Renderer* renderer, RenderPass* pass) {
    if (renderer->passCount < MAX_RENDER_PASSES) {
        renderer->passes[renderer->passCount++] = pass;
    } else {
        fprintf(stderr, "Render pass limit reached.\n");
    }
} 

void RendererRenderFrame(Renderer* renderer) {
    for (size_t i = 0; i < renderer->passCount; i++) {
        RenderPass* prev = (i > 0) ? renderer->passes[i - 1] : NULL;
        renderer->passes[i]->render(renderer->passes[i], prev);
    }
}

void RendererCleanUp(Renderer* renderer) {
    for (size_t i=0; i < renderer->passCount; i++) {
        renderer->passes[i]->destroy(renderer->passes[i]);
    }
}
