#include "renderPasses/geometrypass.h"
#include "renderpass.h"
#include <stdlib.h>

static void GeometryPassRender(RenderPass* self, RenderPass* lastPass) {
    GeometryPass* pass = (GeometryPass*)self;
}

static void GeometryPassDestroy(RenderPass* self) {
    GeometryPass* pass = (GeometryPass*)self;
}

GeometryPass* GeometryPassCreate() {
    GeometryPass* pass = (GeometryPass*)malloc(sizeof(GeometryPass));
    pass->base.name = "GeometryPass";
    pass->base.render = GeometryPassRender;
    pass->base.destroy = GeometryPassDestroy;
    return pass;
}
