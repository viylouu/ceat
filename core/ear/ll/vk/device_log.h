#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

extern VkDevice _ear_vk_device;

void
_ear_vk_create_logical_device(
    void
    );
void
_ear_vk_delete_logical_device(
    void
    );
