#ifndef GBUFFER_H
#define GBUFFER_H
#include "glad/glad.h"

typedef struct GBuffer {
    GLuint fbo;
    GLuint gPosition;
    GLuint gNormal;
    GLuint gMRAE;
    GLuint gDepth;
    GLuint gAlbedo;
    int width, height;
} GBuffer;

GBuffer GBufferCreate();
void GBufferResize(GBuffer* gbuffer, int width, int height);
void GBufferBind(GBuffer* gbuffer);
void GBufferUnbind(GBuffer* gbuffer);
void GBufferDestroy(GBuffer* gbuffer);
#endif
