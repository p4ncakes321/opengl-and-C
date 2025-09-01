#include "renderPasses/geometrypass.h"
#include "camera.h"
#include "gbuffer.h"
#include "renderpass.h"
#include <stdio.h>
#include <stdlib.h>

static void GeometryPassRender(RenderPass* self, RenderPass* lastPass, Camera* camera) {
    GeometryPass* pass = (GeometryPass*)self;
    GBufferBind(&pass->gbuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (size_t i=0; i < pass->objects->count; i++) {
        RenderObject* renderObject = ObjectVector_get(pass->objects, i);
        renderObject->material->bindFunc(
            renderObject->material,
            renderObject->modelMatrix,
            renderObject->instanceCount,
            camera,
            false
        );
        renderObject->mesh->drawFunc(renderObject->mesh, renderObject->modelMatrix, renderObject->instanceCount);
    }

    GBufferUnbind(&pass->gbuffer);
}


static void GeometryPassResize(RenderPass* self, int width, int height) {
    GeometryPass* pass = (GeometryPass*)self;
    GBufferResize(&pass->gbuffer, width, height);
}

static void GeometryPassCleanUp(RenderPass* self) {
    GeometryPass* pass = (GeometryPass*)self;
    ObjectVector_clear(pass->objects);
}

static void GeometryPassDestroy(RenderPass* self) {
    GeometryPass* pass = (GeometryPass*)self;
    ObjectVector_free(pass->objects);
    GBufferDestroy(&pass->gbuffer);
    free(pass);
}

GeometryPass* GeometryPassCreate() {
    GeometryPass* pass = (GeometryPass*)malloc(sizeof(GeometryPass));
    pass->base.name = "GeometryPass";
    pass->base.render = GeometryPassRender;
    pass->base.destroy = GeometryPassDestroy;
    pass->base.cleanup = GeometryPassCleanUp;
    pass->base.resize = GeometryPassResize;
    pass->base.screen_height = 0;
    pass->base.screen_width = 0;
    pass->objects = ObjectVector_create();
    pass->gbuffer = GBufferCreate();
    return pass;
}

void GeometryPassAddObject(GeometryPass* pass, RenderObject object) {
    ObjectVector_push(pass->objects, object);
}

void GeometryPassClearObjects(GeometryPass* pass) {
    ObjectVector_clear(pass->objects);
}

RenderObject RenderObjectCreate(Mesh* mesh, Material* material, mat4* modelMatrix, size_t instanceCount) {
    RenderObject obj;
    obj.mesh = (Mesh*)mesh;
    obj.material = (Material*)material;
    obj.modelMatrix = modelMatrix;
    obj.instanceCount = instanceCount;
    return obj;
}

