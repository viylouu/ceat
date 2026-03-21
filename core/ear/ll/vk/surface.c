#include "surface.h"
#include "../../../cutil.h"

#include "instance.h"

#include "../../../eaw/window.h"
#include <GLFW/glfw3.h>

VkSurfaceKHR _ear_vk_surface;

void
_ear_vk_create_surface(
    void
    ) {
    eat_assert(glfwCreateWindowSurface(_ear_vk_instance, _eaw_glfw_window, NULL, &_ear_vk_surface) == VK_SUCCESS,
        "failed to create window surface!");
}
void
_ear_vk_delete_surface(
    void
    ) {
    vkDestroySurfaceKHR(_ear_vk_instance, _ear_vk_surface, NULL);
}
