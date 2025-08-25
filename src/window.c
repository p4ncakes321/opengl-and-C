#include "glad/glad.h"
#include "window.h"
#include "engine.h"
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void framebuffer_size_callback(GLFWwindow* glfw_window, int width, int height) {
    Window* window = (Window*)glfwGetWindowUserPointer(glfw_window);
    if (!window) return;
    WindowResize(window, width, height);
    if (window->onSizeChanged) window->onSizeChanged(window, width, height);
}

static void cursor_position_callback(GLFWwindow* glfw_window, double x, double y) {
    Window* window = (Window*)glfwGetWindowUserPointer(glfw_window);
    if (!window) return;

    if (window->onMouseMoved) window->onMouseMoved(window, x, y);
}

static void key_callback(GLFWwindow* glfw_window, int key, int scancode, int action, int mods) {
    Window* window = (Window*)glfwGetWindowUserPointer(glfw_window);
    if (!window) return;

    if (window->onKey) window->onKey(window, key, scancode, action, mods);
}

void WindowResize(Window* window, int x, int y) {
    window->width = x;
    window->height = y;
    glViewport(0,0, x, y);
}

Window* WindowCreate(int width, int height, const char* title, GLFWwindow* shareContext) {
    Window* window = calloc(1, sizeof(Window));
    if (!window) return NULL;

    window->title = strdup(title);
    window->handle = glfwCreateWindow(width, height, title, NULL, shareContext);
    window->renderer.passCount = 0;
    if (!window->handle) {
        free(window->title);
        free(window);
        return NULL;
    }

    glfwMakeContextCurrent(window->handle);
    if (!EngineInitializeGlad()) {
        printf("Failed to initialize GLAD\n");
        glfwDestroyWindow(window->handle);
        free(window->title);
        free(window);
        return NULL;
    }

    WindowResize(window, width, height);

    glfwSetWindowUserPointer(window->handle, window);
    glfwSetFramebufferSizeCallback(window->handle, framebuffer_size_callback);
    glfwSetCursorPosCallback(window->handle, cursor_position_callback);
    glfwSetKeyCallback(window->handle, key_callback);

    return window;
}

void WindowDestroy(Window* window) {
    if (!window) return;
    if (window->handle) glfwDestroyWindow(window->handle);
    if (window->title) free(window->title);
    free(window);
}

void WindowSetSizeChangedCallback(Window* window, SizeChangedCallback callback) {
    if (window) window->onSizeChanged = callback;
}

void WindowSetMouseMovedCallback(Window* window, MouseMovedCallback callback) {
    if (window) window->onMouseMoved = callback;
}

void WindowSetKeyCallback(Window* window, KeyCallback callback) {
    if (window) window->onKey = callback;
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
    if (toggle) {
        glEnable(GL_DEPTH_TEST);
    } else {
        glDisable(GL_DEPTH_TEST);
    }
}

void WindowMakeCurrentContext(Window* window) {
    glfwMakeContextCurrent(window->handle);
}
