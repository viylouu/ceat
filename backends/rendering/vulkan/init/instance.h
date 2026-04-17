#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

extern VkInstance _ear_vk_instance;

void 
_ear_vk_create_instance(
    const char* title
    );
void
_ear_vk_delete_instance(
    void
    );
