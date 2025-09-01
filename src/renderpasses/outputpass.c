#include "renderPasses/outputpass.h"
#include "renderpass.h"
#include <stdlib.h>
#include <stdio.h>

static float quadVertices[] = {
    -1.0f, -1.0f,  0.0f, 0.0f,
     1.0f, -1.0f,  1.0f, 0.0f,
     1.0f,  1.0f,  1.0f, 1.0f,

    -1.0f, -1.0f,  0.0f, 0.0f,
     1.0f,  1.0f,  1.0f, 1.0f,
    -1.0f,  1.0f,  0.0f, 1.0f
};

static void OutputPassRender(RenderPass* self, RenderPass* lastPass, Camera* camera) {
    (void)lastPass;
    (void)camera;

    OutputPass* pass = (OutputPass*)self;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);

    ShaderUse(pass->shader);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, pass->gbuffer->gPosition);
    ShaderSetInteger(pass->shader, "gPosition", 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, pass->gbuffer->gNormal);
    ShaderSetInteger(pass->shader, "gNormal", 1);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, pass->gbuffer->gMRAE);
    ShaderSetInteger(pass->shader, "gMRAE", 2);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, pass->gbuffer->gAlbedo);
    ShaderSetInteger(pass->shader, "gAlbedo", 3);

    glBindVertexArray(pass->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);
}

static void OutputPassResize(RenderPass* self, int width, int height) {
    self->screen_width = width;
    self->screen_height = height;
}

static void OutputPassCleanup(RenderPass* self) {
}

void OutputPassDestroy(RenderPass* self) {
    OutputPass* pass = (OutputPass*)self;
    glDeleteVertexArrays(1, &pass->quadVAO);
    glDeleteBuffers(1, &pass->quadVBO);
    ShaderDestroy(pass->shader);
    free(pass);
}

OutputPass* OutputPassCreate(GBuffer* gbuffer) {
    OutputPass* pass = (OutputPass*)malloc(sizeof(OutputPass));
    if (!pass) return NULL;

    pass->base.name = "OutputPass";
    pass->base.render = OutputPassRender;
    pass->base.resize = OutputPassResize;
    pass->base.cleanup = OutputPassCleanup;
    pass->base.destroy = OutputPassDestroy;
    pass->base.screen_width = 0;
    pass->base.screen_height = 0;

    pass->gbuffer = gbuffer;

    glGenVertexArrays(1, &pass->quadVAO);
    glGenBuffers(1, &pass->quadVBO);
    glBindVertexArray(pass->quadVAO);

    glBindBuffer(GL_ARRAY_BUFFER, pass->quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    glBindVertexArray(0);

    pass->shader = ShaderCreate("shaders/quad.vert", "shaders/quad.frag");

    return pass;
}

