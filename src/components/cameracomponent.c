#include "components/cameracomponent.h"
#include <stdlib.h>

CameraComponent* CameraComponentCreate(Camera* camera, size_t instanceIndex) {
    CameraComponent* c = (CameraComponent*)malloc(sizeof(CameraComponent));
    if (!c) return NULL;
    c->camera = camera;
    c->active = true;
    c->instanceIndex = instanceIndex;
    return c;
}

void CameraComponentSetActive(CameraComponent* component, bool value) {
    if (!component) return;
    component->active = value;
}

void CameraComponentSetInstanceIndex(CameraComponent* component, size_t idx) {
    if (!component) return;
    component->instanceIndex = idx;
}

void CameraComponentDestroy(CameraComponent* component) {
}
