#include <cglm/cglm.h>
#include "camera.h"
#include "cameras/perspectivecamera.h"
#include "cameraviews/fullscreen.h"
#include "components/materialComponent.h"
#include "components/meshcomponent.h"
#include "components/transformcomponent.h"
#include "eventmanager.h"
#include "material.h"
#include "mesh.h"
#include "window.h"
#include "engine.h"
#include "renderer.h"
#include "renderPasses/geometrypass.h"
#include "materials/defaultmaterial.h"
#include "meshes/staticmesh.h"
#include "ecs.h"

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
    PerspectiveCamera* camera = PerspectiveCameraCreate((vec3){0.0f,0.0f,2.0f}, 45.0f, 1000.0f, 0.1f, (float)screen_width/screen_height);

    void* resizeHandle = EventManagerSubscribe(window->sizeChanged, resize_listener, NULL);
    void* keyHandle    = EventManagerSubscribe(window->keyEvents, key_listener, NULL);

    GeometryPass* geometryPass = GeometryPassCreate();
    RendererAddPass(&window->renderer, (RenderPass*)geometryPass);
    RendererResize(&window->renderer, screen_width, screen_height);

    WindowDepthTesting(window, true);
    WindowVsync(window, false);

    FullScreenCameraView* view = FullScreenCameraViewCreate((Camera*)camera);
    WindowAttachCameraView(window, (CameraView*)view);

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

    ECS* ecs = ECS_Create();
    Entity cubeEntity = ECS_CreateEntity(ecs);

    MeshComponent* meshComponent = MeshComponentCreate((Mesh*)cubeMesh, 1);
    MaterialComponent* materialComponent = MaterialComponentCreate((Material*)cubeMaterial);
    TransformComponent* transformComponent = TransformComponentCreate((vec3){0.0f,0.0f,0.0f}, (vec3){0.0f,0.0f,0.0f}, (vec3){1.0f,1.0f,1.0f});

    ECS_AddMeshComponent(ecs, cubeEntity, meshComponent); 
    ECS_AddMaterialComponent(ecs, cubeEntity, materialComponent);
    ECS_AddTransformComponent(ecs, cubeEntity, transformComponent);

    double startTime = EngineGetTime();
    double lastFrame = startTime;
    double lastTime = startTime;
    int frames = 0;
    float rotationSpeed = 1.0f;

    while (!WindowShouldClose(window)) {
        EnginePollEvents();

        double currentTime = EngineGetTime();
        double deltaTime = currentTime - lastFrame;

        TransformComponent* tComp = ECS_GetTransformComponent(ecs, cubeEntity);
        tComp->rotations[0][1] += rotationSpeed * deltaTime;
        TransformComponentSetRotationAt(tComp, 0, tComp->rotations[0]);

        RenderObject cubeObject = RenderObjectCreate(
            ECS_GetMeshComponent(ecs, cubeEntity)->mesh,
            ECS_GetMaterialComponent(ecs, cubeEntity)->material,
            ECS_GetTransformComponent(ecs, cubeEntity)->modelMatrices,
            ECS_GetMeshComponent(ecs, cubeEntity)->instanceCount
        );
        GeometryPassAddObject(geometryPass, cubeObject);

        WindowClear(window);
        WindowRenderFrame(window);
        WindowSwapBuffers(window);

        frames++;
        if (currentTime - lastTime >= 1.0) {
            printf("FPS: %d\n", frames);
            frames = 0;
            lastTime = currentTime;
        }
        lastFrame = currentTime;
    }

    ECS_Destroy(ecs);
    WindowDestroy(window);
    EngineTerminate();
    return 0;
}

