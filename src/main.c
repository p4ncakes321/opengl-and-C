#include <cglm/cglm.h>
#include "camera.h"
#include "cameras/perspectivecamera.h"
#include "eventmanager.h"
#include "mesh.h"
#include "window.h"
#include "engine.h"
#include "renderer.h"
#include "renderPasses/geometrypass.h"
#include "materials/defaultmaterial.h"
#include "meshes/staticmesh.h"
static int screen_width = 800;
static int screen_height = 600;

static void key_listener(void* userData, void* eventData) {
    Window* window = (Window*)userData;
    KeyEvent* ev = (KeyEvent*)eventData;

    if (ev->key == GLFW_KEY_ESCAPE && ev->action == GLFW_PRESS) {
        WindowClose(window);
    }
}

static void resize_listener(void* userData, void* eventData) {
    (void)userData;
    ResizeEvent* ev = (ResizeEvent*)eventData;

    screen_width  = ev->width;
    screen_height = ev->height;
}

int main() {
    EngineInit();

    Window* window = WindowCreate(screen_width, screen_height, "Rotating Cube", NULL);
    if (!window) return -1;

    void* resizeHandle = EventManagerSubscribe(window->sizeChanged, resize_listener, NULL);
    void* keyHandle    = EventManagerSubscribe(window->keyEvents, key_listener, NULL);

    GeometryPass* geometryPass = GeometryPassCreate();
    RendererAddPass(&window->renderer, (RenderPass*)geometryPass);

    RendererResize(&window->renderer, screen_width, screen_height);

    WindowDepthTesting(window, true);
    WindowVsync(window, false);

    Vertex vertices[] = {
        // Front face (z = 0.5)
        {{-0.5f,-0.5f, 0.5f},{1,0,0,1},{0,0,1},{0,0},{0,0}},
        {{ 0.5f,-0.5f, 0.5f},{0,1,0,1},{0,0,1},{1,0},{1,0}},
        {{ 0.5f, 0.5f, 0.5f},{0,0,1,1},{0,0,1},{1,1},{1,1}},
        {{-0.5f, 0.5f, 0.5f},{1,1,0,1},{0,0,1},{0,1},{0,1}},

        // Back face (z = -0.5)
        {{-0.5f,-0.5f,-0.5f},{1,0,1,1},{0,0,-1},{1,0},{1,0}},
        {{-0.5f, 0.5f,-0.5f},{0,1,1,1},{0,0,-1},{1,1},{1,1}},
        {{ 0.5f, 0.5f,-0.5f},{1,1,1,1},{0,0,-1},{0,1},{0,1}},
        {{ 0.5f,-0.5f,-0.5f},{0.5,0.5,0.5,1},{0,0,-1},{0,0},{0,0}},

        // Left face (x = -0.5)
        {{-0.5f,-0.5f,-0.5f},{1,0,1,1},{-1,0,0},{0,0},{0,0}},
        {{-0.5f, 0.5f,-0.5f},{0,1,1,1},{-1,0,0},{0,1},{0,1}},
        {{-0.5f, 0.5f, 0.5f},{1,1,0,1},{-1,0,0},{1,1},{1,1}},
        {{-0.5f,-0.5f, 0.5f},{1,0,0,1},{-1,0,0},{1,0},{1,0}},

        // Right face (x = 0.5)
        {{ 0.5f,-0.5f, 0.5f},{0,1,0,1},{1,0,0},{0,0},{0,0}},
        {{ 0.5f, 0.5f, 0.5f},{0,0,1,1},{1,0,0},{0,1},{0,1}},
        {{ 0.5f, 0.5f,-0.5f},{1,1,1,1},{1,0,0},{1,1},{1,1}},
        {{ 0.5f,-0.5f,-0.5f},{0.5,0.5,0.5,1},{1,0,0},{1,0},{1,0}},

        // Top face (y = 0.5)
        {{-0.5f, 0.5f, 0.5f},{1,1,0,1},{0,1,0},{0,0},{0,0}},
        {{ 0.5f, 0.5f, 0.5f},{0,0,1,1},{0,1,0},{1,0},{1,0}},
        {{ 0.5f, 0.5f,-0.5f},{1,1,1,1},{0,1,0},{1,1},{1,1}},
        {{-0.5f, 0.5f,-0.5f},{0,1,1,1},{0,1,0},{0,1},{0,1}},

        // Bottom face (y = -0.5)
        {{-0.5f,-0.5f,-0.5f},{1,0,1,1},{0,-1,0},{0,0},{0,0}},
        {{ 0.5f,-0.5f,-0.5f},{0.5,0.5,0.5,1},{0,-1,0},{1,0},{1,0}},
        {{ 0.5f,-0.5f, 0.5f},{0,1,0,1},{0,-1,0},{1,1},{1,1}},
        {{-0.5f,-0.5f, 0.5f},{1,0,0,1},{0,-1,0},{0,1},{0,1}}
    };
    GLuint indices[] = {
        0,1,2, 0,2,3,       // front
        4,5,6, 4,6,7,       // back
        8,9,10, 8,10,11,    // left
        12,13,14, 12,14,15, // right
        16,17,18, 16,18,19, // top
        20,21,22, 20,22,23  // bottom
    };

    StaticMesh* cubeMesh = StaticMeshCreate(vertices, 24, indices, 36);
    DefaultMaterial* cubeMaterial = DefaultMaterialCreate("assets/crate.jpg");
    PerspectiveCamera* camera = PerspectiveCameraCreate((vec3){0.0f,0.0f,2.0f}, 45.0f, 1000.0f, 0.1f, (float)screen_width/screen_height);

    mat4 model;
    glm_mat4_identity(model);
    glm_translate(model, (vec3){0.0f, 0.0f, 0.0f});
    glm_scale(model, (vec3){1.0f, 1.0f, 1.0f});

    double startTime = EngineGetTime();
    double lastFrame = startTime;
    double lastTime = startTime;
    int frames = 0;

    const float rotation_speed = glm_rad(45.0f);

    while (!WindowShouldClose(window)) {
        EnginePollEvents();

        double currentTime = EngineGetTime();
        double deltaTime = currentTime - lastFrame;

        float elapsed = (float)(currentTime - startTime);
        float angle = elapsed * rotation_speed;

        glm_mat4_identity(model);
        glm_rotate(model, angle, (vec3){0.0f, 1.0f, 0.0f});
        glm_rotate(model, angle * 0.25f, (vec3){1.0f, 0.0f, 0.0f});
        glm_translate(model, (vec3){0.0f, 0.0f, 0.0f});
        glm_scale(model, (vec3){1.0f, 1.0f, 1.0f});

        RenderObject cubeObject = RenderObjectCreate((Mesh*)cubeMesh, (Material*)cubeMaterial, &model, 1);
        GeometryPassAddObject(geometryPass, cubeObject);

        WindowClear(window);
        RendererRenderFrame(&window->renderer, (Camera*)camera);
        WindowSwapBuffers(window);

        frames++;
        if (currentTime - lastTime >= 1.0) {
            printf("FPS: %d\n", frames);
            frames = 0;
            lastTime = currentTime;
        }
        lastFrame = currentTime;
    }

    RendererCleanUp(&window->renderer);
    WindowDestroy(window);
    EngineTerminate();
    return 0;
}

