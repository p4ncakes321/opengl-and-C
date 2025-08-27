#include "cameras/perspectivecamera.h"
#include "camera.h"
#include <cglm/cam.h>
#include <cglm/cglm.h>
#include <stdlib.h>

static void PerspectiveCameraGetPerspectiveMatrix(Camera* camera, mat4 out) {
    PerspectiveCamera* pcamera = (PerspectiveCamera*)camera;
    glm_perspective(glm_rad(pcamera->fov), pcamera->aspectRatio, pcamera->nearZ, pcamera->farZ, out);
}

static void PerspectiveCameraGetScreenSize(Camera* camera, int width, int height) {
    PerspectiveCamera* pcamera = (PerspectiveCamera*)camera;
    pcamera->aspectRatio = (float)width/height;
}

PerspectiveCamera* PerspectiveCameraCreate(vec3 position, float fov, float farZ, float nearZ, float aspectRatio) {
    PerspectiveCamera* camera = (PerspectiveCamera*)malloc(sizeof(PerspectiveCamera));
    CameraInitialize(&camera->base);

    camera->fov = fov;
    camera->farZ = farZ;
    camera->nearZ = nearZ;
    camera->aspectRatio = aspectRatio;
    camera->base.projMatFunc = PerspectiveCameraGetPerspectiveMatrix;
    camera->base.getScrSizeFunc = PerspectiveCameraGetScreenSize;

    glm_vec3_copy(position, camera->base.position);
    return camera;
};
