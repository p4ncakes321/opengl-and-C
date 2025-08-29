#include "camera.h"
#include <cglm/cglm.h>

void CameraInitialize(Camera* cam) {
    glm_vec3_zero(cam->position);
    glm_vec3_zero(cam->forward);
    glm_vec3_zero(cam->right);
    glm_vec3_zero(cam->up);
    glm_vec3_zero(cam->worldUp);

    cam->yaw = -90.0f;
    cam->pitch = 0.0f;

    cam->forward[0] = 0.0f;
    cam->forward[1] = 0.0f;
    cam->forward[2] = -1.0f;

    cam->up[0] = 0.0f;
    cam->up[1] = 1.0f;
    cam->up[2] = 0.0f;

    glm_vec3_copy(cam->up, cam->worldUp);
    glm_vec3_cross(cam->forward, cam->up, cam->right);
    glm_vec3_normalize(cam->right);

    cam->projMatFunc = NULL;
    cam->getScrSizeFunc = NULL;
}

void CameraGetViewMatrix(Camera* camera, mat4 out) {
    vec3 center;
    glm_vec3_add(camera->position, camera->forward, center);
    glm_lookat(camera->position, center, camera->up, out);
}

void CameraViewIntialize(CameraView* cameraView, Camera* camera, Viewport viewport, onViewResize resizeFunc) {
    cameraView->resizeFunc = resizeFunc;
    cameraView->viewport = viewport;
    cameraView->camera = camera;
}
