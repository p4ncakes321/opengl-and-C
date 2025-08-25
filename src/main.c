#include <stdio.h>
#include <cglm/cglm.h>

#include "engine.h"
#include "window.h"
#include "renderer.h"
#include "renderPasses/geometrypass.h"
#include "materials/defaultmaterial.h"
#include "meshes/staticmesh.h"
#include "shader.h"

int main() {
    EngineInit();

    Window* window = WindowCreate(800, 600, "Rotating Cube", NULL);
    if (!window) {
        fprintf(stderr, "Failed to create window\n");
        glfwTerminate();
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    GeometryPass* geometryPass = GeometryPassCreate();
    RendererAddPass(&window->renderer, (RenderPass*)geometryPass);

    Vertex vertices[] = {
        {{-0.5f, -0.5f,  0.5f}, {1,0,0,1}, {0,0,1}, {0,0}, {0,0}},
        {{ 0.5f, -0.5f,  0.5f}, {0,1,0,1}, {0,0,1}, {1,0}, {1,0}},
        {{ 0.5f,  0.5f,  0.5f}, {0,0,1,1}, {0,0,1}, {1,1}, {1,1}},
        {{-0.5f,  0.5f,  0.5f}, {1,1,0,1}, {0,0,1}, {0,1}, {0,1}},
        {{-0.5f, -0.5f, -0.5f}, {1,0,1,1}, {0,0,-1}, {1,0}, {1,0}},
        {{-0.5f,  0.5f, -0.5f}, {0,1,1,1}, {0,0,-1}, {1,1}, {1,1}},
        {{ 0.5f,  0.5f, -0.5f}, {1,1,1,1}, {0,0,-1}, {0,1}, {0,1}},
        {{ 0.5f, -0.5f, -0.5f}, {0.5,0.5,0.5,1}, {0,0,-1}, {0,0}, {0,0}}
    };

    GLuint indices[] = {
        0,1,2, 0,2,3,
        4,5,6, 4,6,7,
        3,2,6, 3,6,5,
        0,4,7, 0,7,1,
        0,3,5, 0,5,4,
        1,7,6, 1,6,2
    };

    StaticMesh* cubeMesh = StaticMeshCreate(vertices, 8, indices, 36);
    DefaultMaterial* cubeMaterial = DefaultMaterialCreate("assets/dumb.png");

    mat4 projection, view;
    glm_perspective(glm_rad(45.0f), 800.0f/600.0f, 0.1f, 100.0f, projection);
    glm_lookat((vec3){0,0,3}, (vec3){0,0,0}, (vec3){0,1,0}, view);

    mat4 cubeModel;
    glm_mat4_identity(cubeModel);

    float rotationAngle = 0.0f;

    while (!WindowShouldClose(window)) {
        EnginePollEvents();
        rotationAngle += 0.02f;

        glm_mat4_identity(cubeModel);
        glm_rotate_y(cubeModel, rotationAngle, cubeModel);
        glm_rotate_x(cubeModel, rotationAngle*0.5f, cubeModel);

        RenderObject cubeObject;
        cubeObject.mesh = (Mesh*)cubeMesh;
        cubeObject.material = (Material*)cubeMaterial;
        cubeObject.modelMatrix = &cubeModel;
        cubeObject.instanceCount = 1;

        GeometryPassAddObject(geometryPass, cubeObject);

        WindowClear(window);

        Shader shader = cubeMaterial->base.deferredShader;
        ShaderUse(shader);
        ShaderSetMat4(shader, "view", view);
        ShaderSetMat4(shader, "projection", projection);

        RendererRenderFrame(&window->renderer);
        WindowSwapBuffers(window);
    }

    RendererCleanUp(&window->renderer);
    WindowDestroy(window);
    EngineTerminate();
    return 0;
}

