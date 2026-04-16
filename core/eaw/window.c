#include "window.h"
#include "../cutil.h"

#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

int32_t _eaw_window_width;
int32_t _eaw_window_height;

GLFWwindow* _eaw_glfw_window;

void
eaw_window_init(
    const char* title,
    int32_t width, int32_t height
    ) {
    //glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);

    eat_assert(glfwInit(), "glfw failed to init");

    _eaw_window_width  = width;
    _eaw_window_height = height;

    //glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    _eaw_glfw_window = glfwCreateWindow(width, height, title, NULL,NULL);
    eat_assert(_eaw_glfw_window != NULL, "failed to create window");
    
    glfwMakeContextCurrent(_eaw_glfw_window);
    glfwSwapInterval(0);

    // widnows
    glfwSetWindowSize(_eaw_glfw_window, width + 1, height);
    glfwSetWindowSize(_eaw_glfw_window, width, height);

    glfwShowWindow(_eaw_glfw_window);
    glfwPollEvents();
}

void 
eaw_window_exit(
    void
    ) {
    glfwDestroyWindow(_eaw_glfw_window);
    glfwTerminate();
}

void
eaw_window_frame(
    void
    ) {
    glfwGetWindowSize(_eaw_glfw_window, &_eaw_window_width, &_eaw_window_height);
}

void
eaw_window_swapbuf(
    void
    ) {
    glfwSwapBuffers(_eaw_glfw_window);
}

bool
eaw_window_is_open(
    void
    ) {
    return !glfwWindowShouldClose(_eaw_glfw_window);
}
