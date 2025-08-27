#ifndef PERSPECTIVECAM_H
#define PERSPECTIVECAM_H

#include "camera.h"

typedef struct PerspectiveCamera {
    Camera base;
    float fov;
    float farZ, nearZ;
    float aspectRatio;
} PerspectiveCamera;

PerspectiveCamera* PerspectiveCameraCreate(vec3 position, float fov, float farZ, float nearZ, float aspectRatio);

#endif
