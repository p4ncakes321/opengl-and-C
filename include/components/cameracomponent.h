#ifndef CAMERA_COMPONENT_H
#define CAMERA_COMPONENT_H

#include "camera.h"
#include <stdbool.h>
#include <stddef.h>

typedef struct CameraComponent {
    Camera* camera;
    bool active;
    size_t instanceIndex;
} CameraComponent;

CameraComponent* CameraComponentCreate(Camera* camera, size_t instanceIndex);

void CameraComponentSetActive(CameraComponent* component, bool value);
void CameraComponentSetInstanceIndex(CameraComponent* component, size_t idx);
void CameraComponentDestroy(CameraComponent* component);

#endif
