#include "window.h"
#include "../cutil.h"

#include "../ear/ll/vk/vk.h"

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
    eat_assert(glfwInit(), "glfw failed to init");

    _eaw_window_width  = width;
    _eaw_window_height = height;

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    _eaw_glfw_window = glfwCreateWindow(width, height, title, NULL,NULL);
    eat_assert(_eaw_glfw_window != NULL, "failed to create window");

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
    uint32_t prevw = _eaw_window_width; uint32_t prevh = _eaw_window_height;
    glfwGetWindowSize(_eaw_glfw_window, &_eaw_window_width, &_eaw_window_height);
    if (prevw != _eaw_window_width || prevh != _eaw_window_height) ear_framebuffer_resize = true;
}

bool
eaw_window_is_open(
    void
    ) {
    return !glfwWindowShouldClose(_eaw_glfw_window);
}
