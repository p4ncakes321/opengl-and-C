#include "gbuffer.h"
#include <stdio.h>
#include <stdlib.h>

GBuffer GBufferCreate() {
    GBuffer g = {0};
    glGenFramebuffers(1, &g.fbo);
    return g;
}

void GBufferResize(GBuffer* g, int width, int height) {
    g->width = width;
    g->height = height;

    glBindFramebuffer(GL_FRAMEBUFFER, g->fbo);

    if (!g->gPosition) glGenTextures(1, &g->gPosition);
    glBindTexture(GL_TEXTURE_2D, g->gPosition);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, g->gPosition, 0);

    if (!g->gNormal) glGenTextures(1, &g->gNormal);
    glBindTexture(GL_TEXTURE_2D, g->gNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, g->gNormal, 0);

    if (!g->gAlbedo) glGenTextures(1, &g->gAlbedo);
    glBindTexture(GL_TEXTURE_2D, g->gAlbedo);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, g->gAlbedo, 0);

    if (!g->gMRAE) glGenTextures(1, &g->gMRAE);
    glBindTexture(GL_TEXTURE_2D, g->gMRAE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, g->gMRAE, 0);

    if (!g->gDepth) glGenTextures(1, &g->gDepth);
    glBindTexture(GL_TEXTURE_2D, g->gDepth);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, g->gDepth, 0);

    GLuint attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
    glDrawBuffers(4, attachments);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        fprintf(stderr, "GBuffer framebuffer not complete!\n");
        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        fprintf(stderr, "Framebuffer status: 0x%x\n", status);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GBufferBind(GBuffer* g) {
    glBindFramebuffer(GL_FRAMEBUFFER, g->fbo);
}

void GBufferUnbind(GBuffer* g) {
    (void)g;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GBufferDestroy(GBuffer* g) {
    glDeleteTextures(1, &g->gPosition);
    glDeleteTextures(1, &g->gNormal);
    glDeleteTextures(1, &g->gMRAE);
    glDeleteTextures(1, &g->gDepth);
    glDeleteTextures(1, &g->gAlbedo);
    glDeleteFramebuffers(1, &g->fbo);
}

