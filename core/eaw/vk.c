#include "vk.h"

#include <GLFW/glfw3.h>

const char** 
_eaw_vk_get_instance_extensions(
    uint32_t* outamt
    ) {
    return glfwGetRequiredInstanceExtensions(outamt);
}
