#include "renderer.h"
#include "camera.h"
#include "renderpass.h"
#include <stdio.h>

void RendererAddPass(Renderer* renderer, RenderPass* pass) {
    if (renderer->passCount < MAX_RENDER_PASSES) {
        renderer->passes[renderer->passCount++] = pass;
    } else {
        fprintf(stderr, "Render pass limit reached.\n");
    }
} 

void RendererRenderFrame(Renderer* renderer, Camera* camera) {
    if (renderer->passCount > 0) {
        RenderPass* pass = renderer->passes[0];
        camera->getScrSizeFunc(camera, pass->screen_width, pass->screen_height);
    }
    for (size_t i = 0; i < renderer->passCount; i++) {
        RenderPass* prev = (i > 0) ? renderer->passes[i - 1] : NULL;
        renderer->passes[i]->render(renderer->passes[i], prev, camera);
    }
}

void RendererCleanUp(Renderer* renderer) {
    for (size_t i=0; i < renderer->passCount; i++) {
        renderer->passes[i]->destroy(renderer->passes[i]);
    }
}

void RendererResize(Renderer* renderer, int screen_width, int screen_height) {
    for (size_t i=0; i < renderer->passCount; i++) {
        renderer->passes[i]->screen_width = screen_width;
        renderer->passes[i]->screen_height = screen_height;
    }
}
