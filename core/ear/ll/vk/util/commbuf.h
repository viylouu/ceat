#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

VkCommandBuffer
_ear_vk_begin_stcomms(
    void
    );
void
_ear_vk_end_stcomms(
    VkCommandBuffer cb
    );
