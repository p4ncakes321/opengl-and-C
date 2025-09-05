#include <cglm/cglm.h>
#include <cglm/types.h>
#include <cglm/util.h>
#include <stdio.h>
#include "camera.h"
#include "cameras/perspectivecamera.h"
#include "cameraviews/fullscreen.h"
#include "components/materialcomponent.h"
#include "components/meshcomponent.h"
#include "components/transformcomponent.h"
#include "eventmanager.h"
#include "material.h"
#include "mesh.h"
#include "renderPasses/outputpass.h"
#include "window.h"
#include "engine.h"
#include "renderPasses/geometrypass.h"
#include "materials/defaultmaterial.h"
#include "meshes/staticmesh.h"
#include "ecs.h"
#include "resourcemanager.h"

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
    PerspectiveCamera* camera = PerspectiveCameraCreate(
        (vec3){0.0f,0.0f,5.0f}, 45.0f, 1000.0f, 0.1f, (float)screen_width/screen_height
    );

    void* resizeHandle = EventManagerSubscribe(window->sizeChanged, resize_listener, window);
    void* keyHandle    = EventManagerSubscribe(window->keyEvents, key_listener, window);

    GeometryPass* geometryPass = GeometryPassCreate();
    OutputPass* outputPass = OutputPassCreate(&geometryPass->gbuffer);
    
    WindowAddRenderPass(window, (RenderPass*)geometryPass);
    WindowAddRenderPass(window, (RenderPass*)outputPass);

    WindowDepthTesting(window, true);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK); 
    glFrontFace(GL_CCW);
    WindowVsync(window, false);

    FullScreenCameraView* view = FullScreenCameraViewCreate((Camera*)camera);
    WindowAttachCameraView(window, (CameraView*)view);

    ResourceManager* rm = ResourceManagerCreate();

    Vertex vertices[] = {
        // Front face (z = +0.5)
        {{-0.5f,-0.5f, 0.5f},{1,0,0,1},{0,0,1},{0,0},{0,0}},
        {{ 0.5f,-0.5f, 0.5f},{0,1,0,1},{0,0,1},{1,0},{1,0}},
        {{ 0.5f, 0.5f, 0.5f},{0,0,1,1},{0,0,1},{1,1},{1,1}},
        {{-0.5f, 0.5f, 0.5f},{1,1,0,1},{0,0,1},{0,1},{0,1}},

        // Back face (z = -0.5)
        {{ 0.5f,-0.5f,-0.5f},{0.5,0.5,0.5,1},{0,0,-1},{1,0},{1,0}},
        {{-0.5f,-0.5f,-0.5f},{1,0,1,1},{0,0,-1},{0,0},{0,0}},
        {{-0.5f, 0.5f,-0.5f},{0,1,1,1},{0,0,-1},{0,1},{0,1}},
        {{ 0.5f, 0.5f,-0.5f},{1,1,1,1},{0,0,-1},{1,1},{1,1}},

        // Left face (x = -0.5)
        {{-0.5f,-0.5f,-0.5f},{1,0,1,1},{-1,0,0},{1,0},{1,0}},
        {{-0.5f,-0.5f, 0.5f},{1,0,0,1},{-1,0,0},{0,0},{0,0}},
        {{-0.5f, 0.5f, 0.5f},{1,1,0,1},{-1,0,0},{0,1},{0,1}},
        {{-0.5f, 0.5f,-0.5f},{0,1,1,1},{-1,0,0},{1,1},{1,1}},

        // Right face (x = +0.5)
        {{ 0.5f,-0.5f, 0.5f},{0,1,0,1},{1,0,0},{1,0},{1,0}},
        {{ 0.5f,-0.5f,-0.5f},{0.5,0.5,0.5,1},{1,0,0},{0,0},{0,0}},
        {{ 0.5f, 0.5f,-0.5f},{1,1,1,1},{1,0,0},{0,1},{0,1}},
        {{ 0.5f, 0.5f, 0.5f},{0,0,1,1},{1,0,0},{1,1},{1,1}},

        // Top face (y = +0.5)
        {{-0.5f, 0.5f, 0.5f},{1,1,0,1},{0,1,0},{0,1},{0,1}},
        {{ 0.5f, 0.5f, 0.5f},{0,0,1,1},{0,1,0},{1,1},{1,1}},
        {{ 0.5f, 0.5f,-0.5f},{1,1,1,1},{0,1,0},{1,0},{1,0}},
        {{-0.5f, 0.5f,-0.5f},{0,1,1,1},{0,1,0},{0,0},{0,0}},

        // Bottom face (y = -0.5)
        {{-0.5f,-0.5f,-0.5f},{1,0,1,1},{0,-1,0},{0,0},{0,0}},
        {{ 0.5f,-0.5f,-0.5f},{0.5,0.5,0.5,1},{0,-1,0},{1,0},{1,0}},
        {{ 0.5f,-0.5f, 0.5f},{0,1,0,1},{0,-1,0},{1,1},{1,1}},
        {{-0.5f,-0.5f, 0.5f},{1,0,0,1},{0,-1,0},{0,1},{0,1}}
    };

    GLuint indices[] = {
        0, 1, 2, 0, 2, 3,
        4, 5, 6, 4, 6, 7,
        8, 9, 10, 8, 10, 11,
        12, 13, 14, 12, 14, 15,
        16, 17, 18, 16, 18, 19,
        20, 21, 22, 20, 22, 23
    };

    StaticMesh* cubeMesh = StaticMeshCreate(vertices, 24, indices, 36);
    ResourceManager_RegisterMesh(rm, "CubeMesh", (Mesh*)cubeMesh);

    DefaultMaterial* cubeMaterial = DefaultMaterialCreate("assets/crate.jpg");
    ResourceManager_RegisterMaterial(rm, "CubeMaterial", (Material*)cubeMaterial);

    ECS* ecs = ECS_Create();
    Entity parentCube = ECS_CreateEntity(ecs);

    vec3 positions[] = {
        {-1.0f, 0.0f, 0.0f},
        { 1.0f, 0.0f, 0.0f}
    };
    vec3 rotations[] = {
        {0.0f,0.0f,0.0f},
        {0.0f,0.0f,0.0f}
    };
    vec3 scales[] = {
        {1.0f,1.0f,1.0f},
        {1.0f,1.0f,1.0f}
    };

    TransformComponent* parentTransform = TransformComponentCreateMultiple(2, positions, rotations, scales);
    MeshComponent* parentMesh = MeshComponentCreate(ResourceManager_GetMesh(rm, "CubeMesh"), 2);
    MaterialComponent* parentMat = MaterialComponentCreate(ResourceManager_GetMaterial(rm, "CubeMaterial"));

    ECS_AddTransformComponent(ecs, parentCube, parentTransform);
    ECS_AddMeshComponent(ecs, parentCube, parentMesh);
    ECS_AddMaterialComponent(ecs, parentCube, parentMat);

    Entity childCube = ECS_CreateEntity(ecs);
    TransformComponent* childTransform = TransformComponentCreate(
        (vec3){5.0f,0.0f,0.0f},
        (vec3){0.0f,glm_rad(90.0f),0.0f},
        (vec3){0.5f,0.5f,0.5f}
    );
    TransformComponentSetInheritIndex(childTransform, 0);

    MeshComponent* childMesh = MeshComponentCreate(ResourceManager_GetMesh(rm, "CubeMesh"), 1);
    MaterialComponent* childMat = MaterialComponentCreate(ResourceManager_GetMaterial(rm, "CubeMaterial"));
    CameraComponent* childCameraComp = CameraComponentCreate((Camera*)camera, 0);

    ECS_AddCameraComponent(ecs, childCube, childCameraComp);
    ECS_AddTransformComponent(ecs, childCube, childTransform);
    ECS_AddMeshComponent(ecs, childCube, childMesh);
    ECS_AddMaterialComponent(ecs, childCube, childMat);
    ECS_SetParent(ecs, childCube, parentCube);

    double startTime = EngineGetTime();
    double lastFrame = startTime;
    double lastTime = startTime;
    int frames = 0;
    float rotationSpeed = 1.0f;

    while (!WindowShouldClose(window)) {
        EnginePollEvents();

        double currentTime = EngineGetTime();
        double deltaTime = currentTime - lastFrame;

        TransformComponent* tParent = ECS_GetTransformComponent(ecs, parentCube);
        tParent->rotations[0][1] += rotationSpeed * deltaTime;
        TransformComponentSetRotationAt(tParent, 0, tParent->rotations[0]);

        Entity entities[] = { parentCube, childCube };
        for (int i = 0; i < 2; ++i) {
            Entity e = entities[i];
            RenderObject obj = RenderObjectCreate(
                ECS_GetMeshComponent(ecs, e)->mesh,
                ECS_GetMaterialComponent(ecs, e)->material,
                ECS_GetWorldTransform(ecs, e),
                ECS_GetMeshComponent(ecs, e)->instanceCount
            );
            GeometryPassAddObject(geometryPass, obj);
        }

        ECS_UpdateCameras(ecs);

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
    ResourceManagerFree(rm);
    WindowDestroy(window);
    EngineTerminate();
    return 0;
}

