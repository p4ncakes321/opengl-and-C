#include "renderer.h"
#include "camera.h"
#include "renderpass.h"
#include <stdio.h>

void RendererAddPass(Renderer* renderer, RenderPass* pass) {
    RenderPassVector_push(renderer->passes, pass);
    pass->resize(pass, renderer->width, renderer->height);
}

void RendererRenderFrame(Renderer* renderer, CameraViewVector* views) {
    for (size_t i = 0; i < views->count; i++) {
        CameraView* view = views->data[i];
        glViewport(view->viewport.x, view->viewport.y, view->viewport.width, view->viewport.height);
        for (size_t j = 0; j < renderer->passes->count; j++) {
            RenderPass* pass = renderer->passes->data[j];
            pass->render(pass, (j > 0 ? renderer->passes->data[j-1] : NULL), view->camera);
        }
    }

    for (size_t i=0; i < renderer->passes->count; i++) {
        RenderPass* pass = renderer->passes->data[i];
        pass->cleanup(pass);
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

void RendererInit(Renderer* renderer, int screen_width, int screen_height) {
    renderer->width = screen_width;
    renderer->height = screen_height;
}

void RendererResize(Renderer* renderer, int screen_width, int screen_height) {
    renderer->width = screen_width;
    renderer->height = screen_height;
    for (size_t i = 0; i < renderer->passes->count; i++) {
        RenderPass* pass = renderer->passes->data[i];
        pass->screen_width  = screen_width;
        pass->screen_height = screen_height;
        pass->resize(pass, screen_width, screen_height);
    }
}

