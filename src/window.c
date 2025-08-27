#include "window.h"
#include "engine.h"
#include "glad/glad.h" 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void glfw_framebuffer_size_cb(GLFWwindow* glfw_window, int width, int height) {
    Window* window = (Window*)glfwGetWindowUserPointer(glfw_window);
    if (!window) return;

    WindowResize(window, width, height);
    RendererResize(&window->renderer, width, height);
    ResizeEvent ev = { window, width, height };
    EventManagerDispatch(window->sizeChanged, &ev);
}

static void glfw_cursor_pos_cb(GLFWwindow* glfw_window, double x, double y) {
    Window* window = (Window*)glfwGetWindowUserPointer(glfw_window);
    if (!window) return;

    MouseMoveEvent ev = { window, x, y };
    EventManagerDispatch(window->mouseMoved, &ev);
}

static void glfw_key_cb(GLFWwindow* glfw_window, int key, int scancode, int action, int mods) {
    Window* window = (Window*)glfwGetWindowUserPointer(glfw_window);
    if (!window) return;

    KeyEvent ev = { window, key, scancode, action, mods };
    EventManagerDispatch(window->keyEvents, &ev);
}

void WindowResize(Window* window, int x, int y) {
    if (!window) return;
    window->width = x;
    window->height = y;
    glViewport(0, 0, x, y);
}

Window* WindowCreate(int width, int height, const char* title, GLFWwindow* shareContext) {
    Window* window = (Window*)calloc(1, sizeof(Window));
    if (!window) return NULL;

    window->title = strdup(title ? title : "Window");
    window->handle = glfwCreateWindow(width, height, window->title, NULL, shareContext);
    window->renderer.passCount = 0;
    if (!window->handle) {
        free(window->title);
        free(window);
        return NULL;
    }

    glfwMakeContextCurrent(window->handle);
    if (!EngineInitializeGlad()) {
        fprintf(stderr, "Failed to initialize GLAD\n");
        glfwDestroyWindow(window->handle);
        free(window->title);
        free(window);
        return NULL;
    }

    WindowResize(window, width, height);

    window->sizeChanged = EventManagerCreate();
    window->mouseMoved  = EventManagerCreate();
    window->keyEvents   = EventManagerCreate();

    glfwSetWindowUserPointer(window->handle, window);
    glfwSetFramebufferSizeCallback(window->handle, glfw_framebuffer_size_cb);
    glfwSetCursorPosCallback(window->handle, glfw_cursor_pos_cb);
    glfwSetKeyCallback(window->handle, glfw_key_cb);

    return window;
}

void WindowDestroy(Window* window) {
    if (!window) return;
    if (window->handle) glfwDestroyWindow(window->handle);
    if (window->title) free(window->title);

    EventManagerFree(window->sizeChanged);
    EventManagerFree(window->mouseMoved);
    EventManagerFree(window->keyEvents);

    free(window);
}

void WindowSwapBuffers(Window* window) {
    if (window && window->handle) glfwSwapBuffers(window->handle);
}

void WindowClose(Window* window) {
    if (window && window->handle) glfwSetWindowShouldClose(window->handle, 1);
}

void WindowClear(Window* window) {
    if (!window) return;
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

int WindowShouldClose(Window* window) {
    return (window && window->handle) ? glfwWindowShouldClose(window->handle) : 1;
}

void WindowVsync(Window* window, bool toggle) {
    if (!window || !window->handle) return;
    WindowMakeCurrentContext(window);
    glfwSwapInterval(toggle ? 1 : 0);
}

void WindowDepthTesting(Window* window, bool toggle) {
    if (!window || !window->handle) return;
    WindowMakeCurrentContext(window);
    if (toggle) glEnable(GL_DEPTH_TEST);
    else glDisable(GL_DEPTH_TEST);
}

void WindowMakeCurrentContext(Window* window) {
    if (!window || !window->handle) return;
    glfwMakeContextCurrent(window->handle);
}

