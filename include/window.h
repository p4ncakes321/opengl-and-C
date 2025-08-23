#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>
#include <stdbool.h>

typedef struct Window Window;

typedef void (*SizeChangedCallback)(Window* window, int width, int height);
typedef void (*MouseMovedCallback)(Window* window, double x, double y);
typedef void (*KeyCallback)(Window* window, int key, int scancode, int action, int mods);

struct Window {
    GLFWwindow* handle;
    int width, height;
    char* title;

    SizeChangedCallback onSizeChanged;
    MouseMovedCallback onMouseMoved;
    KeyCallback onKey;
};

Window* WindowCreate(int width, int height, const char* title, GLFWwindow* shareContext);
void WindowDestroy(Window* window);

void WindowSetSizeChangedCallback(Window* window, SizeChangedCallback callback);
void WindowSetMouseMovedCallback(Window* window, MouseMovedCallback callback);
void WindowSetKeyCallback(Window* window, KeyCallback callback);

void WindowSwapBuffers(Window* window);
void WindowClear(Window* window);
void WindowClose(Window* window);
int WindowShouldClose(Window* window);
void WindowResize(Window* window, int x, int y);
void WindowMakeCurrentContext(Window* window);
void WindowVsync(Window* window, bool toggle);

#endif
