#include "glad/glad.h"
#include "shader.h"
#include "texture.h"
#include "window.h"
#include "engine.h"
#include "mesh.h"
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

static int screen_width = 800;
static int screen_height = 600;

void window_key_callback(Window* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        WindowClose(window);
    };
}

int main(){
    EngineInit();
    Window* window = WindowCreate(screen_width, screen_height, "Window1", NULL);

    // position, color, normal, uv, uv2
    Vertex vertices[] = {
        {{ 0.5f,  0.5f, 0.0f}, {1,1,1,1}, {0,0,1}, {1,1}, {0,0}},
        {{ 0.5f, -0.5f, 0.0f}, {1,1,1,1}, {0,0,1}, {1,0}, {0,0}},
        {{-0.5f, -0.5f, 0.0f}, {1,1,1,1}, {0,0,1}, {0,0}, {0,0}},
        {{-0.5f,  0.5f, 0.0f}, {1,1,1,1}, {0,0,1}, {0,1}, {0,0}}
    };
    unsigned int indices[] = {0, 1, 3, 1, 2, 3};
    
    Mesh* quad = MeshCreate(vertices, 4, indices, 6);
    Texture* texture = TextureCreate("assets/test.jpg", true, true); 
    GLuint shader = ShaderCreate("shaders/default.vert", "shaders/default.frag");

    WindowSetKeyCallback(window, window_key_callback);

    WindowVsync(window, false);

    unsigned int fps = 0;
    double lastCheckedFpsTime = glfwGetTime();
    while(!WindowShouldClose(window)){
        double currentTime = glfwGetTime();
        if (currentTime - lastCheckedFpsTime >= 1) {
            printf("Fps: %d\n", fps);
            fps = 0;
            lastCheckedFpsTime = currentTime;
        }

        WindowMakeCurrentContext(window);
        WindowClear(window);
        
        ShaderUse(shader);
        ShaderSetInteger(shader, "texture0", 0);
        TextureBind(texture, 0);
        MeshDraw(quad);
        WindowSwapBuffers(window);
        EnginePollEvents();
        fps++;
    }

    TextureDestroy(texture);
    ShaderDestroy(shader);
    MeshDestroy(quad);

    WindowDestroy(window);
    EngineTerminate();

    return 0;
}


