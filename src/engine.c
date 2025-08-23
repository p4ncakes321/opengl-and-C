#include "glad/glad.h"
#include "engine.h"
#include <stdio.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>

static Engine* s_engine = NULL;

void EngineInit(void) {
    if (!s_engine) {
        if (!glfwInit()) {
            printf("Failed to initialize GLFW\n");
            free(s_engine);
            s_engine = NULL;
            return;
        }

        s_engine = (Engine*)malloc(sizeof(Engine));
        s_engine->initializedGlad = false;
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    }
}

bool EngineInitializeGlad(void) {
    if (!s_engine) return false;
    if (s_engine->initializedGlad) return true;
    if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        s_engine->initializedGlad = true;
        return true;
    }
    return false;
};

void EnginePollEvents(void) {
    glfwPollEvents();
}

void EngineTerminate(void) {
    if (s_engine) free(s_engine);
    glfwTerminate();
}
