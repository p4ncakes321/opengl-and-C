#include <stdlib.h>
#include <time.h>
#include <cglm/cglm.h>
#include "window.h"
#include "engine.h"
#include "renderer.h"
#include "renderPasses/geometrypass.h"
#include "materials/defaultmaterial.h"
#include "meshes/staticmesh.h"
#include "shader.h"

#define NUM_CUBES 1000000

static int screen_width = 800;
static int screen_height = 600;

void key_callback(Window* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        WindowClose(window);
    } 
}

int main() {
    EngineInit();
    Window* window = WindowCreate(screen_width, screen_height, "Instanced Cubes", NULL);
    if (!window) return -1;

    GeometryPass* geometryPass = GeometryPassCreate();
    RendererAddPass(&window->renderer, (RenderPass*)geometryPass);

    WindowSetKeyCallback(window, key_callback);
    WindowDepthTesting(window, true);
    WindowVsync(window, false);

    Vertex vertices[] = {
        {{-0.5f,-0.5f, 0.5f},{1,0,0,1},{0,0,1},{0,0},{0,0}},
        {{ 0.5f,-0.5f, 0.5f},{0,1,0,1},{0,0,1},{1,0},{1,0}},
        {{ 0.5f, 0.5f, 0.5f},{0,0,1,1},{0,0,1},{1,1},{1,1}},
        {{-0.5f, 0.5f, 0.5f},{1,1,0,1},{0,0,1},{0,1},{0,1}},
        {{-0.5f,-0.5f,-0.5f},{1,0,1,1},{0,0,-1},{1,0},{1,0}},
        {{-0.5f, 0.5f,-0.5f},{0,1,1,1},{0,0,-1},{1,1},{1,1}},
        {{ 0.5f, 0.5f,-0.5f},{1,1,1,1},{0,0,-1},{0,1},{0,1}},
        {{ 0.5f,-0.5f,-0.5f},{0.5,0.5,0.5,1},{0,0,-1},{0,0},{0,0}}
    };

    GLuint indices[] = {
        0,1,2, 0,2,3,  4,5,6, 4,6,7,
        3,2,6, 3,6,5,  0,4,7, 0,7,1,
        0,3,5, 0,5,4,  1,7,6, 1,6,2
    };

    StaticMesh* cubeMesh = StaticMeshCreate(vertices, 8, indices, 36);
    DefaultMaterial* cubeMaterial = DefaultMaterialCreate("assets/crate.jpg");

    mat4 projection, view;
    glm_perspective(glm_rad(45.0f), (float)screen_width/screen_height, 0.1f, 100.0f, projection);
    glm_lookat((vec3){0,0,3}, (vec3){0,0,0}, (vec3){0,1,0}, view);

    mat4* cubeModels = (mat4*)malloc(sizeof(mat4) * NUM_CUBES);
    if (!cubeModels) { fprintf(stderr,"Failed to allocate cube matrices\n"); return -1; }

    srand((unsigned int)time(NULL));
    for (int i = 0; i < NUM_CUBES; i++) {
        glm_mat4_identity(cubeModels[i]);
        float x = ((rand()%2000)/1000.0f - 1.0f) * 2.0f;
        float y = ((rand()%2000)/1000.0f - 1.0f) * 2.0f;
        float z = -((rand()%1000)/1000.0f) * 5.0f;
        glm_translate(cubeModels[i], (vec3){x, y, z});
        glm_scale(cubeModels[i], (vec3){0.05f,0.05f,0.05f});
    }

    float rotationAngle = 0.05f;
    double lastTime = EngineGetTime();
    int frames = 0;

    while (!WindowShouldClose(window)) {
        EnginePollEvents();
        double currentTime = EngineGetTime();
        double deltaTime = currentTime - lastTime;

        for (int i = 0; i < NUM_CUBES; i++)
            glm_rotate_y(cubeModels[i], rotationAngle * 0.5f * deltaTime, cubeModels[i]);

        RenderObject cubeObject;
        cubeObject.mesh = (Mesh*)cubeMesh;
        cubeObject.material = (Material*)cubeMaterial;
        cubeObject.modelMatrix = cubeModels;
        cubeObject.instanceCount = NUM_CUBES;

        GeometryPassAddObject(geometryPass, cubeObject);

        WindowClear(window);

        Shader shader = cubeMaterial->base.deferredShader;
        ShaderUse(shader);
        ShaderSetMat4(shader, "view", view);
        ShaderSetMat4(shader, "projection", projection);

        RendererRenderFrame(&window->renderer);
        WindowSwapBuffers(window);

        frames++;
        if (currentTime - lastTime >= 1.0) {
            printf("FPS: %d\n", frames);
            frames = 0;
            lastTime = currentTime;
        }
    }

    free(cubeModels);
    RendererCleanUp(&window->renderer);
    WindowDestroy(window);
    EngineTerminate();
    return 0;
}
