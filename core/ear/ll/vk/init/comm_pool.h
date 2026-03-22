#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

extern VkCommandPool _ear_vk_comm_pool;

void
_ear_vk_create_command_pool(
    void
    );
void
_ear_vk_delete_command_pool(
    void
    );
