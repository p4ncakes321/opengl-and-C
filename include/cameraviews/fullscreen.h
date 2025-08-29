#ifndef VIEWFULLSCREEN_H
#define VIEWFULLSCREEN_H

#include "camera.h"
typedef struct FullScreenCameraView {
    CameraView base;
} FullScreenCameraView;

FullScreenCameraView* FullScreenCameraViewCreate(Camera* camera);

#endif
