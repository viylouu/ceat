#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "vk.h"

extern VkCommandBuffer _ear_vk_comm_buffers[EAR_VK_MAX_FRAMES_IN_FLIGHT];

void
_ear_vk_create_command_buffers(
    void
    );
