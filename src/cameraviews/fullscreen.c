#include "cameraviews/fullscreen.h"
#include "camera.h"
#include <stdlib.h>

static void FullScreenCameraViewResize(CameraView* view, int width, int height) {
    view->viewport.width = width;
    view->viewport.height = height;

    view->camera->getScrSizeFunc(view->camera, width, height);
}

FullScreenCameraView* FullScreenCameraViewCreate(Camera* camera) {
    FullScreenCameraView* view = (FullScreenCameraView*)malloc(sizeof(FullScreenCameraView));
    Viewport viewport = {0,0,0,0};
    CameraViewIntialize(&view->base, camera, viewport, FullScreenCameraViewResize);
    return view;
}
