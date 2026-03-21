#include "vk.h"
#include "../../../cutil.h"

#include "surface.h"
#include "validation.h"
#include "instance.h"

#include "../../../eaw/eaw.h"
#include <GLFW/glfw3.h>

// TODO: find out where to put this
bool
_eaw_can_create_wl_surface(
    VkInstance inst
    ) {
/*#ifdef _WIN32
    return true;
#else
    if (!getenv("WAYLAND_DISPLAY")) return true;

    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    GLFWwindow* win = glfwCreateWindow(16,16, "dummy", NULL,NULL);
    eat_assert(win != NULL, "failed to create window");

    VkSurfaceKHR surface;
    VkResult r = glfwCreateWindowSurface(inst, win, NULL, &surface);

    if (r == VK_SUCCESS) vkDestroySurfaceKHR(inst, surface, NULL);
    else eat_warn("cannot create wayland surface! falling back to xwayland!");

    glfwDestroyWindow(win);
    return r == VK_SUCCESS;
#endif*/
    return true;
}

void 
ear_vk_init(
    const char* title,
    int32_t width, int32_t height,
    bool vsync
    ) {
    eaw_init(title, width, height, vsync);

    eat_assert(!_ear_vk_validation || _ear_vk_validation_support(), "validation layers unavailable!");

    _ear_vk_create_instance();

    if (!_eaw_can_create_wl_surface(_ear_vk_instance)) {
        _ear_vk_delete_instance();
        eaw_stop();

        glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
        eaw_init(title, width, height, vsync);

        _ear_vk_create_instance();
    }

    _ear_vk_create_surface();
}
void
ear_vk_exit(
    void
    ) {
    _ear_vk_delete_surface();

    _ear_vk_delete_instance();
}
