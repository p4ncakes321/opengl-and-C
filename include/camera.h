#ifndef CAMERA_H
#define CAMERA_H

#include <cglm/cglm.h>

typedef struct Camera Camera;
typedef struct CameraView CameraView;
typedef void (*getProjectionMatrix)(Camera*, mat4 out);
typedef void (*getScreenSize)(Camera*, int width, int height);
typedef void (*onViewResize)(CameraView*, int width, int height);

struct Camera {
    vec3 position;
    vec3 forward;
    vec3 right;
    vec3 up;
    vec3 worldUp;
    float yaw;
    float pitch;
  
    getScreenSize getScrSizeFunc;
    getProjectionMatrix projMatFunc;
};

typedef struct Viewport {
    int x, y;
    int width, height;
} Viewport;

struct CameraView {
    Camera* camera;
    Viewport viewport;
    onViewResize resizeFunc;
};

void CameraInitialize(Camera* cam);
void CameraGetViewMatrix(Camera* cam, mat4 out);
void CameraViewIntialize(CameraView* cameraView, Camera* camera, Viewport viewport, onViewResize resizeFunc);

#endif
