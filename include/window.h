#ifndef WINDOW_H
#define WINDOW_H
#include "renderer.h"
#include "eventmanager.h"
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <stdbool.h>

typedef struct Window Window;

typedef struct {
    Window* window;
    int width;
    int height;
} ResizeEvent;

typedef struct {
    Window* window;
    double x;
    double y;
} MouseMoveEvent;

typedef struct {
    Window* window;
    int key;
    int scancode;
    int action;
    int mods;
} KeyEvent;

struct Window {
    GLFWwindow* handle;
    Renderer renderer;
    int width, height;
    char* title;

    EventManager* sizeChanged;
    EventManager* mouseMoved;
    EventManager* keyEvents;
};

Window* WindowCreate(int width, int height, const char* title, GLFWwindow* shareContext);
void WindowDestroy(Window* window);

void WindowSwapBuffers(Window* window);
void WindowClear(Window* window);
void WindowClose(Window* window);
int WindowShouldClose(Window* window);
void WindowResize(Window* window, int x, int y);
void WindowMakeCurrentContext(Window* window);
void WindowVsync(Window* window, bool toggle);
void WindowDepthTesting(Window* window, bool toggle);

#endif
