#include "renderer.h"
#include "camera.h"
#include "renderpass.h"
#include <stdio.h>

void RendererAddPass(Renderer* renderer, RenderPass* pass) {
    RenderPassVector_push(renderer->passes, pass);
}

void RendererRenderFrame(Renderer* renderer, CameraView* view) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(view->viewport.x, view->viewport.y, view->viewport.width, view->viewport.height);

    for (size_t i = 0; i < renderer->passes->count; i++) {
        RenderPass* prev = (i > 0) ? renderer->passes->data[i - 1] : NULL;
        RenderPass* pass = renderer->passes->data[i];

        pass->screen_height = view->viewport.height;
        pass->screen_width  = view->viewport.width;
        pass->render(pass, prev, view->camera);
    }
}

void RendererFree(Renderer* renderer) {
    if (!renderer || !renderer->passes) return;

    for (size_t i = 0; i < renderer->passes->count; i++) {
        renderer->passes->data[i]->destroy(renderer->passes->data[i]);
    }

    RenderPassVector_free(renderer->passes);
    renderer->passes = NULL;
}

void RendererResize(Renderer* renderer, int screen_width, int screen_height) {
    for (size_t i = 0; i < renderer->passes->count; i++) {
        RenderPass* pass = renderer->passes->data[i];
        pass->screen_width  = screen_width;
        pass->screen_height = screen_height;
    }
}

