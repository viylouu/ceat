#include "window.h"
#include "../cutil.h"

#include <GLFW/glfw3.h>

int32_t _eaw_window_width;
int32_t _eaw_window_height;

GLFWwindow* _eaw_glfw_window;

void
eaw_window_init(
    const char* title,
    int32_t width, int32_t height,
    bool vsync
    ) {
    //glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);

    /*
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    */
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    eat_assert(glfwInit(), "glfw failed to init");

    _eaw_window_width  = width;
    _eaw_window_height = height;

    _eaw_glfw_window = glfwCreateWindow(width, height, title, NULL,NULL);
    eat_assert(_eaw_glfw_window != NULL, "failed to create window");

    //glfwMakeContextCurrent(_eaw_glfw_window);
    //glfwSwapInterval(vsync);

    // widnows
    glfwSetWindowSize(_eaw_glfw_window, width + 1, height);
    glfwSetWindowSize(_eaw_glfw_window, width, height);
}

void 
eaw_window_stop(
    void
    ) {
    glfwDestroyWindow(_eaw_glfw_window);
    glfwTerminate();
}

void
eaw_window_frame(
    void
    ) {
    //glfwSwapBuffers(_eaw_glfw_window);

    glfwGetWindowSize(_eaw_glfw_window, &_eaw_window_width, &_eaw_window_height);
}

bool
eaw_window_is_open(
    void
    ) {
    return !glfwWindowShouldClose(_eaw_glfw_window);
}
