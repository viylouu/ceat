#include "window.h"
#include "../cutil.h"

#include "../../eat.h"

#include <GLFW/glfw3.h>

int32_t eaw_window_width;
int32_t eaw_window_height;

GLFWwindow* eaw_window;

void
eaw_window_init(
    const char* title,
    int32_t width, int32_t height,
    bool vsync
    ) {
    //glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);

    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    eat_assert(glfwInit(), "glfw failed to init");

    eaw_window_width  = width;
    eaw_window_height = height;

    eaw_window = glfwCreateWindow(width, height, title, NULL,NULL);
    eat_assert(eaw_window != NULL, "failed to create window");

    glfwMakeContextCurrent(eaw_window);
    glfwSwapInterval(vsync);

    // widnows
    glfwSetWindowSize(eaw_window, width + 1, height);
    glfwSetWindowSize(eaw_window, width, height);
}

void 
eaw_window_stop(
    void
    ) {
    glfwDestroyWindow(eaw_window);
    glfwTerminate();
}

void
eaw_window_frame(
    void
    ) {
    glfwSwapBuffers(eaw_window);

    int32_t prev_width = eaw_window_width;
    int32_t prev_height = eaw_window_height;
    glfwGetWindowSize(eaw_window, &eaw_window_width, &eaw_window_height);

    if (prev_width != eaw_window_width || prev_height != eaw_window_height) {
        ear_resize_texture(_eat_screen_color, eaw_window_width, eaw_window_height);
        ear_resize_texture(_eat_screen_depth, eaw_window_width, eaw_window_height);
        ear_resize_framebuffer(_eat_screen_framebuffer, eaw_window_width, eaw_window_height);
    }
}

bool
eaw_window_is_open(
    void
    ) {
    return !glfwWindowShouldClose(eaw_window);
}
