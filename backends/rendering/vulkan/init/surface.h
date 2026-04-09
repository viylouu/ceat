#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

extern VkSurfaceKHR _ear_vk_surface;

void
_ear_vk_create_surface(
    void
    );
void
_ear_vk_delete_surface(
    void
    );
